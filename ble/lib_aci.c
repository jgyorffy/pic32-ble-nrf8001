/**

 *  Author: John Gyorffy
 *  UART specific driver to transfer data to and from the peer using BLE
 */

#include "hal_platform.h"
#include "aci.h"
#include "aci_cmds.h"
#include "aci_evts.h"
#include "lib_aci.h"
#include "services.h"
#include "hal_aci_tl.h"
#include "aci_queue.h"


static hal_aci_data_t init_data_to_send[NB_SETUP_MESSAGES] = SETUP_MESSAGES_CONTENT;
static hal_aci_data_t received_data = {0};
static hal_aci_data_t data_to_send = {0};
static aci_evt_t aci_evt = {0};
static uint8_t total_credits = 0;
static on_connected pconnected_clbk = NULL;
static on_connected pdisconnected_clbk = NULL;
static on_data_received pon_data_clbk = NULL;
static on_system_error on_system_error_clbk = NULL;
static volatile uint8_t remaining_credits = 0;
static uint8_t *pDeviceName = NULL;
static uint8_t *pModelNo = NULL;
static uint8_t *pRevNo = NULL;
static uint8_t *pManufacturerName = NULL;
static uint8_t *pSystemId = NULL;

static bool get_any_event(aci_evt_t* paci_evt);
static bool lib_message_pump(void);
static bool lib_aci_set_app_latency(uint16_t latency, aci_app_latency_mode_t latency_mode);
static bool lib_aci_sleep();
static bool lib_aci_radio_reset();
static bool lib_aci_device_version();
static bool lib_aci_connect(uint16_t run_timeout, uint16_t adv_interval);
static bool lib_aci_disconnect(aci_disconnect_reason_t reason);
static bool lib_aci_wakeup();
static bool lib_aci_set_tx_power(aci_device_output_power_t tx_power);
static bool lib_aci_get_temperature();
static bool lib_aci_get_battery_level();
static bool lib_aci_send_data(uint8_t pipe, uint8_t *p_value, uint8_t size);
static bool lib_aci_change_timing(uint16_t minimun_cx_interval, uint16_t maximum_cx_interval, uint16_t slave_latency, uint16_t timeout);
static bool lib_aci_change_timing_GAP_PPCP();
static bool lib_aci_set_local_data(uint8_t pipe, uint8_t *p_value, uint8_t size);



static void __ISR(_CORE_SOFTWARE_0_VECTOR, ipl4) swi_message_pump(void) {

    CoreClearSoftwareInterrupt0();
    INTClearFlag(INT_CS0);
    while (lib_message_pump());
}

static void __ISR(_CORE_SOFTWARE_1_VECTOR, ipl2) swi_data_rcved(void) {
    aci_evt_t *paci_evt = NULL;
    CoreClearSoftwareInterrupt1();
    INTClearFlag(INT_CS1);
    paci_evt = (aci_evt_t*) aci_queue_dequeue_waiting();
    if (paci_evt != NULL && (paci_evt->len - 2) > 0) {
        pon_data_clbk(paci_evt->params.data_received.rx_data.aci_data, 
                        paci_evt->len - 2,
                        paci_evt->params.data_received.rx_data.pipe_number);    
    }

}

static void bleEventSWIOn() {
    INTSetVectorPriority(INT_CORE_SOFTWARE_0_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_CORE_SOFTWARE_0_VECTOR, INT_SUB_PRIORITY_LEVEL_1);
    INTClearFlag(INT_CS0);
    INTEnable(INT_CS0, INT_ENABLED);
    CoreClearSoftwareInterrupt0();

    INTSetVectorPriority(INT_CORE_SOFTWARE_1_VECTOR, INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_CORE_SOFTWARE_1_VECTOR, INT_SUB_PRIORITY_LEVEL_1);
    INTClearFlag(INT_CS1);
    INTEnable(INT_CS1, INT_ENABLED);
    CoreClearSoftwareInterrupt1();
}

static uint8_t wait_specific_cmd_event(hal_aci_data_t * pdata, aci_cmd_opcode_t cmd_opcode) {
    aci_evt_t* paci_evt = NULL;

    while (!m_aci_spi_transfer(pdata, &received_data));
    while (true) {
        paci_evt = (aci_evt_t*) aci_queue_dequeue_waiting();
        if (paci_evt != NULL) {
            if (paci_evt->params.cmd_rsp.cmd_opcode == cmd_opcode) {
                return paci_evt->params.cmd_rsp.cmd_status;
            }
        }
    }

    return ACI_STATUS_ERROR_UNKNOWN;
}

static bool wait_any_isr_event(aci_evt_t* paci_evt) {
    uint8_t *pReturn = NULL;
    uint16_t i;

    for (i = 0; i < 60000; i++) {
        pReturn = aci_queue_dequeue_isr();
        if (pReturn != NULL) {
            //memcpy(received_data.buffer, pReturn, 32);
            *paci_evt = *((aci_evt_t*) pReturn);
            if (paci_evt->evt_opcode != ACI_EVT_INVALID) {
                return true;
            }
        }
        DelayMs(1);
    }

    return false;
}

static bool get_any_event(aci_evt_t* paci_evt) {

    memset(paci_evt, 0x00, HAL_ACI_MAX_LENGTH + 2);
    uint8_t *pReturn = NULL;

    pReturn = aci_queue_dequeue_isr();

    if (pReturn != NULL) {
        *paci_evt = *((aci_evt_t*) pReturn);
        memcpy(received_data.buffer, pReturn, 32);

        if (paci_evt->evt_opcode != ACI_EVT_INVALID) {
            return true;
        }
    }
    return false;
}

static bool setup_pipes() {
    //switch on sw interrupts to handle events
    bleEventSWIOn();

    if (pDeviceName) {
        if (!lib_aci_set_local_data(PIPE_GAP_DEVICE_NAME_SET, pDeviceName,
                PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE)) {
            return false;
        }
    }
    if (pModelNo) {
        if (!lib_aci_set_local_data(PIPE_DEVICE_INFORMATION_MODEL_NUMBER_STRING_SET, pModelNo,
                PIPE_DEVICE_INFORMATION_MODEL_NUMBER_STRING_SET_MAX_SIZE)) {
            return false;
        }
    }
    if (pRevNo) {
        if (!lib_aci_set_local_data(PIPE_DEVICE_INFORMATION_HARDWARE_REVISION_STRING_SET, pRevNo,
                PIPE_DEVICE_INFORMATION_HARDWARE_REVISION_STRING_SET_MAX_SIZE)) {
            return false;
        }
    }
    if (pManufacturerName) {
        if (!lib_aci_set_local_data(PIPE_DEVICE_INFORMATION_MANUFACTURER_NAME_STRING_SET, pManufacturerName,
                PIPE_DEVICE_INFORMATION_MANUFACTURER_NAME_STRING_SET_MAX_SIZE)) {
            return false;
        }
    }
    if (pSystemId) {
        if (!lib_aci_set_local_data(PIPE_DEVICE_INFORMATION_SYSTEM_ID_SET, pSystemId,
                PIPE_DEVICE_INFORMATION_SYSTEM_ID_SET_MAX_SIZE)) {
            return false;
        }
    }

    if (!lib_aci_set_tx_power(ACI_DEVICE_OUTPUT_POWER_0DBM)) {
        return false;
    }

    return true;
}

bool ble_board_init() {
    aci_evt_t aci_evt = {0};
    uint8_t i;
    bool retVal = false;

    //setup the ports and interrupt on the physical MCU
    hal_aci_tl_init();
    //Send the soft reset to get the nRF8001 to a known state.
    lib_aci_radio_reset();
    DelayMs(1000);
    //make sure it started ok
    retVal = wait_any_isr_event(&aci_evt);
    if (retVal) {
        if (ACI_EVT_DEVICE_STARTED != aci_evt.evt_opcode ||
                aci_evt.params.device_started.hw_error != ACI_HW_ERROR_NONE) {
            return false;
        }
    } else {
        return false;
    }

    remaining_credits = total_credits = aci_evt.params.device_started.credit_available;

    //now send the setup messages as defined by nrfGo services.h
    for (i = 0; i < NB_SETUP_MESSAGES; i++) {
        m_aci_spi_transfer(&init_data_to_send[i], &received_data);
        do {
            retVal = wait_any_isr_event(&aci_evt);
            if (!retVal) {
                return false;
            }
            if (ACI_EVT_CMD_RSP == aci_evt.evt_opcode &&
                    (ACI_STATUS_TRANSACTION_CONTINUE == aci_evt.params.cmd_rsp.cmd_status ||
                    ACI_STATUS_TRANSACTION_COMPLETE == aci_evt.params.cmd_rsp.cmd_status)) {
                break;
            }
        } while (retVal);
    }
    do {
        retVal = wait_any_isr_event(&aci_evt);
        if (ACI_EVT_DEVICE_STARTED == aci_evt.evt_opcode &&
                ACI_DEVICE_STANDBY == aci_evt.params.device_started.device_mode) {
            return setup_pipes();
        }
    } while (retVal);

    return false;
}

bool ble_connect() {

    if (lib_aci_connect(0x0000, 0x0050)) {
        return true;
    }

    return false;
}

void ble_uart_send(uint8_t *pData, uint8_t size) {
    while (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, pData, size));
}

void ble_device_name(uint8_t *szDeviceName) {
    pDeviceName = szDeviceName;
}

void ble_set_model_no(uint8_t *szModelNo) {
    pModelNo = szModelNo;
}

void ble_set_rev_no(uint8_t *szrevNo) {
    pRevNo = szrevNo;
}

void ble_manufacturer_name(uint8_t *szManufacturerName) {
    pManufacturerName = szManufacturerName;
}

void ble_system_id(uint8_t *szSystemId) {
    pSystemId = szSystemId;
}

void ble_set_connected_callback(on_connected pfncclbk) {
    pconnected_clbk = pfncclbk;
}

void ble_set_disconnected_callback(on_disconnected pfncclbk) {
    pdisconnected_clbk = pfncclbk;
}

void ble_set_on_data_handler(on_data_received pfncclbk) {
    pon_data_clbk = pfncclbk;
}

void ble_set_system_error_callback(on_system_error pfncclbk) {
    on_system_error_clbk = pfncclbk;
}

static bool lib_message_pump() {

    if (get_any_event(&aci_evt)) {

        switch (aci_evt.evt_opcode) {

            case ACI_EVT_CONNECTED:
                remaining_credits = total_credits;
                //memcpy(&connected_params, &aci_evt.params.connected, sizeof (connected_params));
                if (pconnected_clbk) pconnected_clbk();
                break;
            case ACI_EVT_PIPE_STATUS:
                //memcpy(&pipe_status, &aci_evt.params.pipe_status, sizeof (pipe_status));
                break;
            case ACI_EVT_DISCONNECTED:
                remaining_credits = 0;
                if (pdisconnected_clbk) pdisconnected_clbk();
                break;
            case ACI_EVT_DATA_CREDIT:
                remaining_credits += aci_evt.params.data_credit.credit;
                break;
            case ACI_EVT_DATA_RECEIVED:
                if (pon_data_clbk) {
                    aci_queue_enqueue_waiting(&aci_evt);
                    CoreSetSoftwareInterrupt1();
                }
                break;
            case ACI_EVT_TIMING:
                //connection_interval = aci_evt.params.timing.conn_rf_interval;
                //slave_latency = aci_evt.params.timing.conn_slave_rf_latency;
                //supervision_timeout = aci_evt.params.timing.conn_rf_timeout;
                break;
            case ACI_EVT_PIPE_ERROR:
            case ACI_EVT_HW_ERROR:
                if (on_system_error_clbk) on_system_error_clbk();
                break;
            case ACI_EVT_CMD_RSP:
                aci_queue_enqueue_waiting(&aci_evt);
                break;
            default:
                break;
        }
        return true;
    }

    return false;
}

bool lib_aci_set_app_latency(uint16_t latency, aci_app_latency_mode_t latency_mode) {
    hal_aci_data_t data_to_send = {0};
    aci_cmd_params_set_app_latency_t aci_set_app_latency;

    aci_set_app_latency.mode = latency_mode;
    aci_set_app_latency.latency = latency;
    acil_encode_cmd_set_app_latency(&(data_to_send.buffer[0]), &aci_set_app_latency);
    return m_aci_spi_transfer(&data_to_send, &received_data);
}

bool lib_aci_sleep() {
    hal_aci_data_t data_to_send = {0};
    acil_encode_cmd_sleep(&(data_to_send.buffer[0]));
    return m_aci_spi_transfer(&data_to_send, &received_data);
}

bool lib_aci_radio_reset() {
    hal_aci_data_t data_to_send = {0};
    acil_encode_baseband_reset(data_to_send.buffer);
    return m_aci_spi_transfer(&data_to_send, &received_data);
}

bool lib_aci_connect(uint16_t run_timeout, uint16_t adv_interval) {
    aci_cmd_params_connect_t aci_cmd_params_connect = {0};
    aci_cmd_params_connect.timeout = run_timeout;
    aci_cmd_params_connect.adv_interval = adv_interval;
    acil_encode_cmd_connect(data_to_send.buffer, &aci_cmd_params_connect);
    if (wait_specific_cmd_event(&data_to_send, ACI_CMD_CONNECT) == ACI_STATUS_SUCCESS) {
        return true;
    }
    return false;
}

bool lib_aci_disconnect(aci_disconnect_reason_t reason) {
    aci_cmd_params_disconnect_t aci_cmd_params_disconnect = {0};
    aci_cmd_params_disconnect.reason = reason;
    acil_encode_cmd_disconnect(&data_to_send.buffer[0], &aci_cmd_params_disconnect);
    if (wait_specific_cmd_event(&data_to_send, ACI_CMD_DISCONNECT) == ACI_STATUS_SUCCESS) {
        return true;
    }
    return false;
}

bool lib_aci_wakeup() {
    acil_encode_cmd_wakeup(&data_to_send.buffer[0]);
    if (wait_specific_cmd_event(&data_to_send, ACI_CMD_WAKEUP) == ACI_STATUS_SUCCESS) {
        return true;
    }
    return false;
}

bool lib_aci_set_tx_power(aci_device_output_power_t tx_power) {
    aci_cmd_params_set_tx_power_t aci_cmd_params_set_tx_power = {0};
    aci_cmd_params_set_tx_power.device_power = tx_power;
    acil_encode_cmd_set_radio_tx_power(&data_to_send.buffer[0], &aci_cmd_params_set_tx_power);
    if (wait_specific_cmd_event(&data_to_send, ACI_CMD_SET_TX_POWER) == ACI_STATUS_SUCCESS) {
        return true;
    }
    return false;
}

bool lib_aci_get_temperature() {
    hal_aci_data_t data_to_send = {0};
    acil_encode_cmd_temparature(&(data_to_send.buffer[0]));
    return m_aci_spi_transfer(&data_to_send, &received_data);
}

bool lib_aci_get_battery_level() {
    hal_aci_data_t data_to_send = {0};
    acil_encode_cmd_battery_level(&(data_to_send.buffer[0]));
    return m_aci_spi_transfer(&data_to_send, &received_data);
}

bool lib_aci_send_data(uint8_t pipe, uint8_t *p_value, uint8_t size) {
    aci_cmd_params_send_data_t aci_cmd_params_send_data = {0};

    if (size > ACI_PIPE_TX_DATA_MAX_LEN) {
        return false;
    }

    if (remaining_credits == 0) {
        return false;
    }

    remaining_credits--;

    aci_cmd_params_send_data.tx_data.pipe_number = pipe;
    memcpy(aci_cmd_params_send_data.tx_data.aci_data, p_value, size);
    acil_encode_cmd_send_data(data_to_send.buffer, &aci_cmd_params_send_data, size);
    while(!m_aci_spi_transfer(&data_to_send, &received_data));

    return true;
}

bool lib_aci_change_timing(uint16_t minimun_cx_interval, uint16_t maximum_cx_interval, uint16_t slave_latency, uint16_t timeout) {
    hal_aci_data_t data_to_send = {0};
    aci_cmd_params_change_timing_t aci_cmd_params_change_timing;
    aci_cmd_params_change_timing.conn_params.min_conn_interval = minimun_cx_interval;
    aci_cmd_params_change_timing.conn_params.max_conn_interval = maximum_cx_interval;
    aci_cmd_params_change_timing.conn_params.slave_latency = slave_latency;
    aci_cmd_params_change_timing.conn_params.timeout_mult = timeout;
    acil_encode_cmd_change_timing_req(&(data_to_send.buffer[0]), &aci_cmd_params_change_timing);
    return m_aci_spi_transfer(&data_to_send, &received_data);
}

bool lib_aci_change_timing_GAP_PPCP() {
    hal_aci_data_t data_to_send = {0};
    acil_encode_cmd_change_timing_req_GAP_PPCP(&(data_to_send.buffer[0]));
    return m_aci_spi_transfer(&data_to_send, &received_data);
}

bool lib_aci_set_local_data(uint8_t pipe, uint8_t *p_value, uint8_t size) {
    aci_cmd_params_set_local_data_t aci_cmd_params_set_local_data = {0};

    aci_cmd_params_set_local_data.tx_data.pipe_number = pipe;
    memcpy(&aci_cmd_params_set_local_data.tx_data.aci_data[0], p_value, size);
    acil_encode_cmd_set_local_data(&data_to_send.buffer[0], &aci_cmd_params_set_local_data, size);
    if (wait_specific_cmd_event(&data_to_send, ACI_CMD_SET_LOCAL_DATA) == ACI_STATUS_SUCCESS) {
        return true;
    }
    return false;
}


