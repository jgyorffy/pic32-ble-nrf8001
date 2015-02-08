#ifndef LIB_ACI_H__
#define LIB_ACI_H__

/**
-Public BLE APIs for a raw data transfer device
*/

#include "hal_platform.h"
//callbacks for events
typedef void (*on_connected)(void);
typedef void (*on_system_error)(void);
typedef void (*on_disconnected)(void);
typedef void (*on_data_received)(const uint8_t *, uint8_t len, uint8_t pipeNum);

//user APIs
bool ble_board_init();
void ble_device_name(uint8_t *szDeviceName);
void ble_set_model_no(uint8_t *szModelNo);
void ble_set_rev_no(uint8_t *szrevNo);
void ble_manufacturer_name(uint8_t *szManufacturerName);
void ble_system_id(uint8_t *szSystemId);
bool ble_connect();
void ble_set_connected_callback(on_connected);
void ble_set_system_error_callback(on_system_error);
void ble_set_on_data_handler(on_data_received);
void ble_uart_send(uint8_t *pData, uint8_t size);

#endif 
