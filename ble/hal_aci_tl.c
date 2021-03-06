/** @file
@brief Implementation of the ACI transport layer module
 *
 * Author: John Gyorffy
 * Simplified the code and added a more generic interupt callback mechanism
 */
#include "lib_aci.h"
#include "hal_platform.h"
#include "hal_aci_tl.h"
#include "aci_queue.h"


/*
PIC32 supports only MSbit transfer on SPI and the nRF8001 uses LSBit
Use the REVERSE_BITS macro to convert from MSBit to LSBit
The outgoing command and the incoming event needs to be converted
 */

static bool volatile sending_data = false;

//For PIC: the transmission has to be reversed, the next definitions have to be added
#define REVERSE_BITS(byte) (((reverse_lookup[(byte & 0x0F)]) << 4) + reverse_lookup[((byte & 0xF0) >> 4)])
static const uint8_t reverse_lookup[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};


static void hal_aci_tl_pin_reset(void);
static uint8_t spi_readwrite(uint8_t aci_byte);

static hal_aci_data_t data_to_send = {0};
static hal_aci_data_t received_data = {0};

/*
  Interrupt service routine called when the RDYN line goes low. Runs the SPI transfer.
 */
ble_rdy_irq_begin

while (1) {
    // handle the interrupt when data is ready
    if (m_aci_spi_transfer(&data_to_send, &received_data)) {
        break;
    }
}

CoreSetSoftwareInterrupt0();

ble_rdy_irq_end

void hal_aci_tl_init() {

    /* Needs to be called as the first thing for proper intialization*/
    setup_gpio();

    ble_spi_open();

    /* Pin reset the nRF8001, required when the nRF8001 setup is being changed */
    hal_aci_tl_pin_reset();

    DelayMs(100); //Wait for the nRF8001 to get hold of its lines - the lines float for a few ms after the reset

    ble_enable_rdy_int();
}

bool m_aci_spi_transfer(hal_aci_data_t* data_to_send, hal_aci_data_t* received_data) {
    uint8_t byte_cnt = 0;
    uint8_t byte_sent_cnt = 0;
    uint8_t volatile rdyn = 1;
    uint8_t payload_count = 0;

    ble_disable_rdy_int();

    if (sending_data) {
        return false;
    }
    sending_data = true;

    memset(received_data->buffer, 0x00, HAL_ACI_MAX_LENGTH);

    enable_request();
    //have to delay reqn line low until rdyn pin is low (40ns min in doc)
    Delayus(100);
    rdyn = get_ready_pin();
    while (rdyn) {
        rdyn = get_ready_pin();
    }
    //delay before SPI clocking (100 ns min in doc)
    Delayus(1);

    // Send length, receive header
    byte_sent_cnt = 0;
    received_data->status_byte = spi_readwrite(data_to_send->buffer[byte_sent_cnt++]);
    // Send first byte, receive length from slave
    received_data->buffer[0] = spi_readwrite(data_to_send->buffer[byte_sent_cnt++]);

    if (received_data->buffer[0] > HAL_ACI_MAX_LENGTH) {
        payload_count = data_to_send->buffer[0];
    } else {
        payload_count = received_data->buffer[0] > data_to_send->buffer[0] ?
                received_data->buffer[0] : data_to_send->buffer[0];
    }

    //now clock in the payload if length > 0 and less than max
    if (payload_count > 0 && payload_count <= HAL_ACI_MAX_LENGTH) {
        for (byte_cnt = 0; byte_cnt < payload_count; byte_cnt++) {
            received_data->buffer[byte_cnt + 1]
                    = spi_readwrite(data_to_send->buffer[byte_sent_cnt++]);
        }
    }

    if (received_data->buffer[0] > 0 && received_data->buffer[0] <= HAL_ACI_MAX_LENGTH) {
        aci_queue_enqueue_isr(received_data);
    }

    // RDYN should follow the REQN line in approx 100ns
    disable_request();
    sending_data = false;
    ble_enable_rdy_int();

    return true;
}

static void hal_aci_tl_pin_reset(void) {
    reset_high();
    reset_low();
    reset_high();
}

static inline uint8_t spi_readwrite(const uint8_t aci_byte) {
    ble_write_spi(REVERSE_BITS(aci_byte));
    while (!ble_spi_read_buf_full()); // wait for TX complete
    return REVERSE_BITS(ble_read_spi()); // read the received values
}

