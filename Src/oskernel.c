/*
 * oskernel.c
 *
 *  Created on: Sep 22, 2023
 *      Author: Malefetsane Lenka
 */
#include "oskernel.h"
uint32_t MS_PRESCALER;    /* milli seconds prescaler*/

/*Task Control Block*/
struct tcb{
	uint32_t *stackPt;    /*task stack pointer*/
	struct tcb *nextPt;  /*next task tcb*/
	uint32_t block_count;
	uint8_t current_state;
};

typedef struct tcb tcbType;
tcbType tcbs[NUM_OF_THREADS]; /*Array of tasks control block*/
/*Each thread has stacksize of 400 bytes*/
uint32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];
tcbType *currentPt;  /*current thread tcb*/
tcbType *head;      /*head is pointer to idle task tcb*/
__attribute__((naked)) static void osSchedulerLaunch(void);
static void osKernelStackInit(int i);
static void update_ticks(void);


uint32_t g_tick_count;    /*global tick count*/
uint32_t curr_tick_count; /*current tick count*/
uint8_t tick_freq    = 1;


static void osKernelStackInit(int i)
{
	tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE-16];    /*stack pointer*/
	/*Full descending stack, last element on highest memory address*/
	TCB_STACK[i][STACKSIZE-1] = THUMBBIT_SET; /*xPSR 24th bit is always 1*/
	/*dummy stack content for other registers except PC*/
	for (uint8_t j=3; j<17; j++){
		/* LR{R14},R12,R3,R2,R1 first then R10-R4*/
		TCB_STACK[i][STACKSIZE-j] = 0xAAAABBBB; /*dummy data*/
	}
}

uint8_t osKernelAddThreads(void(*threads[])(void)){
	/*disable global interrupts*/
	__disable_irq();
	int i=0;
	for (;i<(NUM_OF_THREADS-1); i++){
		tcbs[i].nextPt = &tcbs[i+1];  /*linked list of threads*/
	}
	head = &tcbs[0];
	tcbs[NUM_OF_THREADS-1].nextPt = &tcbs[0]; /*tcb of last thread points to tcb of first thread*/
	i = 0;
	for(;i<NUM_OF_THREADS; i++){
		osKernelStackInit(i); /*Initialize stack for each task*/
		tcbs[i].current_state = TASK_READY_STATE;

		TCB_STACK[i][STACKSIZE-2] = (uint32_t)threads[i];  /*set PC to the address of each thread function*/
	}
	currentPt = &tcbs[1]; /*Start from thread1*/

	/*enable global interrupts*/
	__enable_irq();

	return 1;

}

void osKernelInit(void){
	MS_PRESCALER = (AHB_FREQ/1000);
	NVIC_SetPriority(PendSV_IRQn,15);
}


void osKernelLaunch(uint32_t quanta)
{
	/*Reset SysTick*/
	SysTick->CTRL = SysTick_Reset;
	/*clear SysTick current value register*/
	SysTick->VAL = 0;
	/*Load quanta*/
	SysTick->LOAD = (quanta*MS_PRESCALER)-1;
	/*Set SysTick to Low priority*/
	NVIC_SetPriority(SysTick_IRQn,15);
	/*select AHB as SysTick clock and SysTick exception*/
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk;
	/*Enable SysTick*/
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	/*Launch Scheduler*/
	osSchedulerLaunch();
}


void SysTick_Handler(void){
		update_ticks();
		unblock_threads();
		SCB->ICSR |= 1U<<28; /*pend PendSV*/


}

static void update_ticks(){
	g_tick_count+=tick_freq;
}

__attribute__((naked))void PendSV_Handler(void){
		/*1.suspend current thread*/

		/*disable global interrupts*/
		__asm volatile("CPSID I");
		/*save r4-r11*/
		__asm volatile("PUSH {R4-R11}");
		/*load currentPt to R0*/
		__asm volatile("LDR R0, =currentPt");
		/*load address of current thread tcb to R1*/
		__asm volatile("LDR R1,[R0]");
		/*store cortex-M stack to current Thread tcb stackpointer*/
		__asm volatile("STR SP,[R1]");

	  /*2. choose next thread*/
		__asm volatile("PUSH {R0,R1,LR}");
		__asm volatile("BL update_next_thread");
		__asm volatile("POP {R0,R1,LR}");

		/*R1 = next updated thread*/
		__asm volatile("LDR R1,[R0]");

		/*SP = currentPt->StackPt*/
		__asm volatile("LDR SP,[R1]");

		/*restore r4-r11*/
		__asm volatile("POP {R4-R11}");

		/*enable global interrupts*/

		__asm volatile("CPSIE I");

		/*return from exception and restore saved stack frame*/
		__asm volatile("BX LR");
}


__attribute__((naked)) static void osSchedulerLaunch(void)
{
	   /*disable global interrupts*/
		__asm volatile("CPSID I");
		__asm volatile("LDR R0,=currentPt");
		/*load address of current thread to r2*/
		__asm volatile("LDR R2, [R0]");
		/*set SP = currentPt->Stackpt*/
		__asm volatile("LDR SP,[R2]");
		/*restore r4-r11*/
		__asm volatile("POP {R4-R11}");
		/*restore r0-r3*/
		__asm volatile("POP {R0-R3}");
		/*restore r12*/
		__asm volatile("POP {R12}");
		/*skip LR*/
		__asm volatile("ADD SP,SP,#4");
		/*load PC to LR*/
		__asm volatile("POP {LR}");
		/*Skip xPSR*/
		__asm volatile("ADD SP,SP,#4");
		/*enable global interrupts*/
		__asm volatile("CPSIE I");
		/*return from subroutine*/
		__asm volatile("BX LR");
}


uint32_t getTick(void){
	__disable_irq();
	curr_tick_count = g_tick_count;
	__enable_irq();
	return curr_tick_count;

}

/*
 * @Brief: update the currentPt to next ready thread
 * @Param: None
 * @Retval: None
 */
void update_next_thread(void){
    uint32_t state=TASK_BLOCKED_STATE;
    do{
    	currentPt=currentPt->nextPt;
    	state=currentPt->current_state;
    	if(state==TASK_READY_STATE){
    		break;
    	}

    }while(currentPt!=head);
}

/*
 * @Brief: unblock any ready task/thread except for idle task
 * @Param: None
 * @Retval: None
 */
void unblock_threads(void){
   for (uint32_t i=1; i<NUM_OF_THREADS; i++){
       if (tcbs[i].current_state !=TASK_READY_STATE){
           if(tcbs[i].block_count==g_tick_count){
               tcbs[i].current_state =TASK_READY_STATE;
           }
       }
   }
}


/*
 * @Brief: Add a delay tick_count to the thread
 * @Param: tick_count for desired delay in ms
 * @Retval: None
 * @Note: thread will be blocked for QUANTA*tick_count
 */
void thread_delay(uint32_t tick_count){
  __asm volatile("CPSID I");
  if (currentPt!=head){
          currentPt->block_count = getTick()+tick_count;
          currentPt->current_state = TASK_BLOCKED_STATE;
          SCB->ICSR |= 1U<<28; /*pend PendSV*/
  }
   __asm volatile("CPSIE I");
}
