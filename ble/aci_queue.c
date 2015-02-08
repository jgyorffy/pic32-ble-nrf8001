/*
*  Author: John Gyorffy
 * Circular queue to seperate data between interrupts and prevent race conditions
 */
#include "hal_platform.h"
#include "aci.h"
#include "aci_cmds.h"
#include "aci_evts.h"
#include "acilib_if.h"
#include "hal_aci_tl.h"
#include "lib_aci.h"
#include "aci_queue.h"

static hal_aci_data_t isr_data[ACI_QUEUE_SIZE] = {0};
static aci_evt_t waiting_data[ACI_QUEUE_SIZE] = {0};
static uint8_t volatile curr_isr_read_pointer = 0;
static uint8_t volatile curr_isr_write_pointer = 0;
static uint8_t volatile curr_waiting_read_pointer = 0;
static uint8_t volatile curr_waiting_write_pointer = 0;

uint8_t * aci_queue_dequeue_isr() {
    hal_aci_data_t *p_temp = NULL;

    if (curr_isr_write_pointer == curr_isr_read_pointer ||
            curr_isr_write_pointer >= ACI_QUEUE_SIZE) {
        return NULL;
    }

    p_temp = &isr_data[curr_isr_read_pointer++];

    if (!(curr_isr_read_pointer % ACI_QUEUE_SIZE)) {
        curr_isr_read_pointer = 0;
    }

    return p_temp->buffer;
}

void aci_queue_enqueue_isr(hal_aci_data_t *p_data) {

    isr_data[curr_isr_write_pointer++] = *p_data;

    if (!(curr_isr_write_pointer % ACI_QUEUE_SIZE)) {
        curr_isr_write_pointer = 0;
    }

}

aci_evt_t * aci_queue_dequeue_waiting() {
    aci_evt_t *p_temp = NULL;

    if (curr_waiting_write_pointer == curr_waiting_read_pointer ||
            curr_waiting_write_pointer >= ACI_QUEUE_SIZE) {
        return NULL;
    }

    p_temp = &waiting_data[curr_waiting_read_pointer++];

    if (!(curr_waiting_read_pointer % ACI_QUEUE_SIZE)) {
        curr_waiting_read_pointer = 0;
    }

    return p_temp;
}

void aci_queue_enqueue_waiting(aci_evt_t *p_data) {

    waiting_data[curr_waiting_write_pointer++] = *p_data;

    if (!(curr_waiting_write_pointer % ACI_QUEUE_SIZE)) {
        curr_waiting_write_pointer = 0;
    }

}