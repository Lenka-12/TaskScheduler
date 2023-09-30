# RoundRobin Scheduler using Circular LinkedList

This project is an implementation of a simple Real-Time Operating System (RTOS) scheduler for an ARM Cortex-M microcontroller (specifically STM32F411xE). The scheduler allows for multitasking and context switching between multiple tasks. Each task has its own stack and is scheduled to run based on time quanta provided by a system tick timer.


## Tasks
The project defines four user tasks and 1 idle task:

1. Task0 (`task0`): Toggles the GPIO pin PD12 at a delay of 1000 ms.
2. Task1 (`task1`): Toggles the GPIO pin PD13 at a delay of 500 ms.
3. Task2 (`task2`): Toggles the GPIO pin PD14 at a delay of 250 ms.
4. Task3 (`task3`): Toggles the GPIO pin PD15 at a delay of 125 ms.

An idle task (`idle_task`) is also included, which runs when no other tasks are ready to execute.



## Scheduler and Context Switching
The scheduler uses a PendSV (Pendable request for system service) interrupt to perform context switching between tasks. The scheduler is implemented in the `PendSV_Handler` function. Context switching is based on the execution states of the tasks (ready or blocked).


## System Tick Timer
The system tick timer generates periodic interrupts, which are used to keep track of time and unblock tasks waiting for a specific delay. The SysTick interrupt is handled in the `SysTick_Handler` function.


## Usage
To use this RTOS scheduler, you need to include the necessary files in your project and configure the GPIO pins according to your hardware setup. Make sure to edit number of threads/tasks in oskernel.h , NUM_OF_THREADS, and the size of stack for each at by chnaging 
STACKSIZE in  oskernel.h file.
 
 Note that number of threads include idle thread, To make sure that idle task is not neglected it is recommended that idle task is not changed in main.c, and it should remain at 0th index of the threads array.

 Task_Profilers are used to quickly check if the tasks are running during debugging, to watch them just add each task profiler to `Live Expression view, when debugging and then hit run with no break point.

## License
This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).

## Contact
For any inquiries or questions, please contact [Malefetsane Lenka](mailto:malefetsanelenka@gmail.com).

