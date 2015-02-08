/*
 * File:   common.h
 * Author: john
 *
 * Created on January 15, 2015, 7:33 PM
 */

#ifndef COMMON_H
#define	COMMON_H
#define _SUPPRESS_PLIB_WARNING 1
#include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#include <plib.h>        /* Include to use PIC32 peripheral libraries      */
#include <stdint.h>      /* For uint32_t definition                        */
#include <stdbool.h>     /* For true/false definition                      */
#include <p32xxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define SYS_FREQ   (80000000ul)
#define PBCLK      (40000000ul)
#define GetPeripheralClock()       (SYS_FREQ / (1<<OSCCONbits.PBDIV) )
#define TT_PER_US                  (40)

#define	config_gpio_dir_o(Port, Pin)   m ## Port ##SetPinsDigitalOut(Pin)
#define	config_gpio_dir_i(Port, Pin)   m ## Port ##SetPinsDigitalIn(Pin)
#define	set_gpio_high(Port, Pin)       m ## Port ## SetBits(Pin)
#define	set_gpio_low(Port, Pin)        m ## Port ## ClearBits(Pin)
#define	set_gpio_toggle(Port, Pin)     m ## Port ## ToggleBits(Pin)
#define	input_get(Port, Pin)           m ## Port ## ReadBits(Pin)
#define	clear_gpio_o(Port, Pin)        m ## Port ##SetPinsDigitalOut(Pin);set_gpio_low(Port, Pin)


#endif