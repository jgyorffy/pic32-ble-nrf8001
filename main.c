/* 
 * File:   main.c
 * Author: John Gyorffy
 *
 * Created on December 7, 2014
 *
 * The purpose of re-writing the nRF8001 BLE module for Arduino is to reduce the
 * code footprint and complexity so that it will be easier to extend.
 * A lot of the code from Nordic was thrown away (or re-written), as it bloated the code space
 * with code targeted for the Arduino. This code was leaned down specifically for any PIC 32
 * MCU. This code is specific for a UART application whereby bulk data is transfered between the
 * devices. The payload can be anything from configuration data to music.
 * BLE, however, has a low tranfer rate since you can only send 20-byte packets.
 * But...only the other hand, it is low battery usage if you keep it in sleep mode
 * and only connected when needed.
 *
 * The communication modules use event-driven mechanisms to make the code more
 * flexible. Polling is not used after startup. All data flows from interrupts.
 *
 * NOTE: The connection timer here is just to aid in re-connecting after the device
 * diconnects. This is not energy efficient to always be transmitting.
 * Design your projects with that in mind.
 *
 *
 */
#include "ble/lib_aci.h"
#include "ble/hal_platform.h"
#include "common.h"
#include "ble/services.h"

// Configuration Bit settings
// SYSCLK = Crystal Freq  / FPLLIDIV * FPLLMUL / FPLLODIV
// PBCLK = SYSCLK / FPBDIV
// SYSCLK = 80 MHz 
// PBCLK = 40 MHz
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2, DEBUG = ON

static uint8_t data_to_send[20] = {0};
//put these in Flash in the future
static uint8_t systemId[PIPE_DEVICE_INFORMATION_SYSTEM_ID_SET_MAX_SIZE] = {38, 37, 36, 35, 34, 33, 32, 31};
static uint8_t deviceName[PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE] = "My BLE Device";
static uint8_t manufacturerName[PIPE_DEVICE_INFORMATION_MANUFACTURER_NAME_STRING_SET_MAX_SIZE] = "ACME ELEC. INC";
static uint8_t modelNo[PIPE_DEVICE_INFORMATION_MODEL_NUMBER_STRING_SET_MAX_SIZE] = "1234567";
static uint8_t hwRev[PIPE_DEVICE_INFORMATION_HARDWARE_REVISION_STRING_SET_MAX_SIZE] = "1.0.0";

static volatile uint8_t timer_ticks;

static void connectionTimerOn() {
    timer_ticks = 0;
    CloseTimer1();
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, 0xFFFF);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_1);
    WriteTimer1(0);
}

static void connectionTimerOff() {
    timer_ticks = 0;
    CloseTimer1();
}

//ISR to reconnect
static void __ISR(_TIMER_1_VECTOR, ipl1) Timer1Handler(void) {
    timer_ticks++;
    //if the device disconnects, then reconnect in about 5 seconds
    if (timer_ticks > 5) {
        connectionTimerOff();
        ble_connect();
    }

    mT1ClearIntFlag();
}

void on_connected_evt(void) {
    //connected to a peer
    connectionTimerOff();

}

void on_disconnected_evt(void) {
 //peer disconnected
    connectionTimerOn();
}

void on_system_error_evt(void) {
  //system error
}

void on_data_handler(const uint8_t *pdata, uint8_t len, uint8_t pipeNum) {
    //got data on a channel ..just send back the same data
    memset(data_to_send, 0x00, 20);
    sprintf(data_to_send, "%s got %d bts", modelNo, len);
    ble_uart_send(data_to_send, 20);
    Delayus(50); //need to sleep between sending 20 bytes
    memset(data_to_send, 0x00, 20);
    memcpy(data_to_send, pdata, len);
    ble_uart_send(data_to_send, 20);
}

void main(void) {
    SYSTEMConfig(SYS_FREQ, SYS_CFG_ALL);
    SYSTEMConfigPerformance(SYS_FREQ);
    // Set peripheral clock to what you need
    mOSCSetPBDIV(OSC_PB_DIV_2); 
    //we have many interrupts
    INTEnableSystemMultiVectoredInt();

    // Set all analog pins to be digital I/O (not necessary for this project)
    AD1PCFG = 0xFFFF;

    //setup the BLE callbacks
    ble_set_connected_callback(on_connected_evt);
    ble_set_disconnected_callback(on_disconnected_evt);
    ble_set_on_data_handler(on_data_handler);
    ble_set_system_error_callback(on_system_error_evt);
    //setup the characteristic values we want to advertise
    ble_device_name(deviceName);
    ble_set_model_no(modelNo);
    ble_set_rev_no(hwRev);
    ble_manufacturer_name(manufacturerName);
    ble_system_id(systemId);
    //init and connect on startup. (you can connect at any time)
    if (ble_board_init() && ble_connect()) {
        //we are ready to go

    }

    while (1) {
       //your main program loop here
    }

}