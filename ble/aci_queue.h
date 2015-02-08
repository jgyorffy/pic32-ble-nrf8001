/** @file
 * @brief Interface for buffer.
 */

/** @defgroup aci_queue aci_queue
@{
@ingroup aci_queue

*/

#ifndef ACI_QUEUE_H__
#define ACI_QUEUE_H__

#include "aci.h"
#include "hal_aci_tl.h"

/***********************************************************************    */
/* The ACI_QUEUE_SIZE determines the memory usage of the system.            */
/* Successfully tested to a ACI_QUEUE_SIZE of 4 (interrupt) and 4 (polling) */
/***********************************************************************    */
#define ACI_QUEUE_SIZE  10

/** Data type for queue of data packets to send/receive from radio.
 *
 *  A FIFO queue is maintained for packets. New packets are added (enqueued)
 *  at the tail and taken (dequeued) from the head. The head variable is the
 *  index of the next packet to dequeue while the tail variable is the index of
 *  where the next packet should be queued.
 */
uint8_t * aci_queue_dequeue_isr(void);
void aci_queue_enqueue_isr(hal_aci_data_t *p_data);


#endif /* ACI_QUEUE_H__ */
/** @} */