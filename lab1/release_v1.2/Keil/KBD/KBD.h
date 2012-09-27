/*----------------------------------------------------------------------------
 * Name:    KBD.h
 * Purpose: MCB1700 low level Joystick definitions
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

#ifndef __KBD_H
#define __KBD_H

#define KBD_SELECT      0x01   //            
#define KBD_LEFT        0x08   // P1.23            
#define KBD_UP          0x10   // P1.24            
#define KBD_RIGHT       0x20   // P1.25            
#define KBD_DOWN        0x40   // P1.26
#define KBD_MASK        0x79  

#define POS_SELECT 0
#define POS_LEFT 1
#define POS_UP 2
#define POS_RIGHT 3
#define POS_DOWN 4
#define NUM_POS 5	// number of positions

extern uint32_t KBD_val;             

extern void     KBD_init(void);
extern void		KBD_set (int joy_pos);
extern uint32_t KBD_get (void);
extern uint32_t INT0_get(void);


#endif
