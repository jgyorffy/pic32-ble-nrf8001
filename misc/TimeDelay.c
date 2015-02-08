/*
*  Author: John Gyorffy
 * Uses the core timer to have more accurate wait states
 */
#include "../common.h"

// <editor-fold defaultstate="collapsed" desc="comment">
// ===========================================================================
// ShortDelay - Delays (blocking) for a very short period (in CoreTimer Ticks)
// ---------------------------------------------------------------------------
// The DelayCount is specified in Core-Timer Ticks.
// This function uses the CoreTimer to determine the length of the delay.
// The CoreTimer runs at half the system clock.
// If CPU_CLOCK_HZ is defined as 80000000UL, 80MHz/2 = 40MHz or 1LSB = 25nS).
// Use US_TO_CT_TICKS to convert from uS to CoreTimer Ticks.
// ---------------------------------------------------------------------------
// </editor-fold>

void Delayus(uint32_t usdelay) {
    uint32_t beginState = ReadCoreTimer();
    uint32_t ttcount = usdelay *= TT_PER_US;
    uint32_t startTicks = 0;
    WriteCoreTimer(0);
    startTicks = ReadCoreTimer();
    while ((ReadCoreTimer() - startTicks) < ttcount) {};
    WriteCoreTimer(beginState);
}

void DelayMs(unsigned int msec) {
    Delayus(msec * 1000);
}