/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************
 * Design Notes:
 *
 * Interrupt Controller:
 *   Intel 8259A Compatiable
 *
 * IRQ list:
 *  -- PIC1 --
 *   0 - timer interrupt
 *   1 - keyboard, mouse, RTC interrupt
 *   2 - video interrupt
 *   3 - serial port 2 interrupt
 *   4 - serial port 1 interrupt
 *   5 - fixed disk interrupt
 *   6 - diskket interrupt
 *   7 - parallel printer interrupt
 *  -- PIC2 --
 *   8 - real-time clock interrupt
 *   9 - redirect cascade
 *  10 - reserved
 *  11 - reserved
 *  12 - mouse interrupt
 *  13 - coprocessor exception interrupt
 *  14 - fixed disk interrupt
 *  15 - reserved
 *
 * References:
 *   Ralf Brown's Interrupt List
 *****************************************************************************/

