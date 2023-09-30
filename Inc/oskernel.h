/*
 * oskernel.h
 *
 *  Created on: Sep 22, 2023
 *      Author: Malefetsane Lenka
 */

#ifndef OSKERNEL_H_
#define OSKERNEL_H_

#define NUM_OF_THREADS               5
#define STACKSIZE                    1024
#define THUMBBIT_SET                (1UL<<24)

#define TASK_BLOCKED_STATE          0b00
#define TASK_READY_STATE            0b01


#define AHB_FREQ                    16000000
#define SysTick_Reset               0x0UL

#include <stdint.h>
#include "stm32f411xe.h"

/******************************
 * our osKernel APIs
 ******************************/
uint8_t osKernelAddThreads(void(*threads[])(void));
void osKernelInit(void);
void osKernelLaunch(uint32_t quanta);
uint32_t getTick(void);
void update_next_thread(void);
void unblock_threads(void);
void thread_delay(uint32_t tick_count);

#endif /* OSKERNEL_H_ */
