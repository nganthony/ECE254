/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: MCB1700 low level LED functions
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

#include <RTL.h>
#include <LPC17xx.H>                    /* LPC17xx definitions                */
#include "LED.h"

const U8 led_pos[8] = { 28, 29, 31, 2, 3, 4, 5, 6 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_init (void) {
  LPC_GPIO1->FIODIR |= 0xB0000000;                 /* LEDs on PORT1  */
  LPC_GPIO2->FIODIR |= 0x0000007C;                 /* LEDs on PORT2  */
}


/*----------------------------------------------------------------------------
  Switch LED on. LED = Bitposition
 *----------------------------------------------------------------------------*/
void LED_on  (int led) {
  U32 mask;

  mask = 1 << led_pos[led];
  if (led < 3) LPC_GPIO1->FIOSET = mask;
  else         LPC_GPIO2->FIOSET = mask;
}

/*----------------------------------------------------------------------------
  Switch Led off. LED = Bitposition
 *----------------------------------------------------------------------------*/
void LED_off (int led) {
  U32 mask;

  mask = 1 << led_pos[led];
  if (led < 3) LPC_GPIO1->FIOCLR = mask;
  else         LPC_GPIO2->FIOCLR = mask;
}

/*----------------------------------------------------------------------------
  Output value to LEDs
 *----------------------------------------------------------------------------*/
void LED_out (int led) {
  int i;
  U32 mask;

  for (i = 0; i < LED_NUM; i++) {
    mask = 1 << led_pos[i];
    if (led & (1<<i)) {
      if (i < 3) LPC_GPIO1->FIOSET = mask;
      else       LPC_GPIO2->FIOSET = mask;
    }
    else {
      if (i < 3) LPC_GPIO1->FIOCLR = mask;
      else       LPC_GPIO2->FIOCLR = mask;
    }
  }
}
