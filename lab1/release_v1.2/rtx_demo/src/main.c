#include <LPC17xx.h>
#include <RTL.h>
#include "GLCD.h"
#include "LED.h"

int g_counter = 0;  // a global counter


__task void task1(void)
{
	int i = 0;
	GLCD_DisplayString(3, 0, 1, "Task 1:");

	for (;;i++) {
		GLCD_DisplayChar(3, 7, 1, i+'0');
		os_dly_wait(100);
		if (i == 9) {
			i = -1;
		}
	}
}


__task void task2(void)
{  
	for (;;) {
		LED_on(7);
		os_dly_wait (60);
		LED_off(7);
		os_dly_wait(40);
	}
}

__task void task3(void)
{
	for (;;) {
		g_counter++;
	}
}

__task void task4(void)
{
	for (;;) {
		g_counter--;
	}
}

__task void init(void)
{
	//os_tsk_create(task1, 1);  // task 1 at priority 1
	//os_tsk_create(task2, 1);  // task 2 at priority 1
	os_tsk_create(task3, 1);  // task 3 at priority 1
	os_tsk_create(task4, 1);  // task 4 at priority 1
	os_tsk_delete_self();     //task must delete itself before exiting
}

int main(void)
{
	SystemInit();         // initialize the LPC17xx MCU
	/*
	LED_init();           // initialize the LED device
	GLCD_Init();          // initialize the LCD device

	LED_on(0);            // turn on LED #0 at P1.28

	GLCD_Clear(Yellow);   // clear LCD screen with yellow color
	GLCD_DisplayString(0,0,1,"Hello World!");
	                      // row 0, col 0, font size 1
	*/
	os_sys_init(init);	  // initilize the OS and start the first task
}
