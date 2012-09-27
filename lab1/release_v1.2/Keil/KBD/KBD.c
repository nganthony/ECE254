/*----------------------------------------------------------------------------
 * Name:    kbd.c
 * Purpose: MCB1700 low level Joystick, INT0 functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.H>                    /* LPC17xx definitions                */
#include "kbd.h"

uint32_t KBD_val  = 0;             
uint32_t INT0_val = 0;         

const uint8_t g_joy_pos[5] = {20, 23, 24, 25, 26}; // SEELCT, LEFT, UP, RIGHT, DOWN

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void KBD_init (void) {

  LPC_PINCON->PINSEL4 &= ~(3<<20);               /* P2.10 is GPIO (INT0) */
  LPC_GPIO2->FIODIR   &= ~(1<<10);               /* P2.10 is input */

  LPC_PINCON->PINSEL3 &= ~((3<< 8)|(3<<14)|(3<<16)|(3<<18)|(3<<20)); /* P1.20, P1.23..26 is GPIO (Joystick) */
  LPC_GPIO1->FIODIR   &= ~((1<<20)|(1<<23)|(1<<24)|(1<<25)|(1<<26)); /* P1.20, P1.23..26 is input */
}


/*----------------------------------------------------------------------------
  Get Joystick value
  //kbd_val = ~KBD_DEFINES & KBD_MASK
  //KBD_SELECT 0x01, kbd_val = ~0x01  & 0x79 = 0x78 = 120
  //KBD_LEFT   0x08, kbd_val = ~0x08  & 0x79 = 0x71 = 113
  //KBD_UP     0x10, kbd_val = ~0x10  & 0x79 = 0x69 = 105
  //KBD_RIGHT  0x20, kbd_val = ~0x20  & 0x79 = 0x59 = 89
  //KBD_DOWN   0x40, kbd_wla = ~0x40  & 0x79 = 0x39 = 57
 *----------------------------------------------------------------------------*/
uint32_t KBD_get  (void) {
  uint32_t kbd_val;
  
  kbd_val = (LPC_GPIO1->FIOPIN >> 20) & KBD_MASK;
  return (kbd_val);
}

void KBD_set(int joy_pos) {
	 
	uint32_t mask;

	mask = 1 << g_joy_pos[joy_pos];
	LPC_GPIO1->FIOSET = mask;
	  
}

void KBD_clear(void)
{
	uint32_t mask;
	int i;
	for (i=0; i<NUM_POS; i++) {
		mask = 1 << g_joy_pos[i];
		LPC_GPIO1->FIOCLR = mask;
	}
}

/*----------------------------------------------------------------------------
  Get INT0 value
 *----------------------------------------------------------------------------*/
uint32_t INT0_get  (void) {
  uint32_t int0_val;

  int0_val = (LPC_GPIO2->FIOPIN >> 10) & 0x01;
  return (int0_val);
}
