/* 
 * File:   header.h
 * Author: nva1773@gmail.com
 *
 * Created on 11/19/2019 4:54:48 PM UTC
 * "Created in MPLAB Xpress"
 */

#ifndef HEADER_H
#define	HEADER_H

/** A S M ***********************************************************/
#define Nop() {__asm("nop");}
#define ClrWdt() {__asm("clrwdt");}
#define Sleep() {__asm("sleep");}
#define Reset() {__asm("reset");}
#define Retfie() {__asm("retfie");}
#define Swapf(reg) {reg = (reg >> 4) | (reg << 4);}

/** O S C ***********************************************************/
// OSC = 4 MHz
#define OSCILLATOR	4000000
#define INSTRUCTION_CYCLE (OSCILLATOR)/4

/** P O R T S ******************************************************/
// Analog - GP0 (Pin7) - Adj 0...1024
// Comparator - GP1 (Pin6) - Sync, ? ?????????? ?????? ? ????????? ??????,
//              ????????? ????????? ???????? ?????????? ?? 1,04 V.
// Input - Enable on GP2 (Pin5), Enable = 0
// Output Pulse on GP4 and GP5 (Pin2 and Pin3) - Log0 = On;
// IOCB - Off, WPU -Off
#define mInitPorts() {CMCON = 0b00010100;		\
                      VRCON = 0b10100101;       \
					  GPIO = 0b11111111;        \
					  TRISIO = 0b11001111;      \
                      GPIO = 0b11111111;        \
					  IOCB = 0x00;              \
					  WPU = 0x00;}
#define isEnable !GP2

/** P I N S *********************************************************/
#define mStartOUT() GPIO = 0xCF;
#define mStopOUT() GPIO = 0xFF;

/** I S R  *********************************************************/
// Interrupt for: 
#define mInterruptEnable()  PEIE = 1; GIE = 1
#define mInterruptDisable() PEIE = 0; GIE = 0

// Timer0
#define mFlagTMR0 T0IF
#define mInterruptTMR0 T0IE&&T0IF
// Timer1
#define mFlagTMR1 TMR1IF
#define mInterruptTMR1 TMR1IE&&TMR1IF

/** T I M E R 0 *****************************************************/
// Fosc/4, prescale 1:64, WDT prescale off: - TMR0 - 10ms = 100Hz
#define TMR0L TMR0
#define mInitTimer0() OPTION_REG = 0b01010101
#define mLoadTimer0() TMR0 = 0x65
#define mStartTimer0() T0IF = 0; T0IE = 1
#define mStopTimer0() T0IF = 0; T0IE = 0

/** T I M E R 1 *****************************************************/
// Fosc/4, prescale 1:1
#define mInitTimer1() T1CON = 0b00000000; TMR1IF = 0
#define mLoadTimer1(a) {TMR1 = 0xFFFF - a;}
#define mStartTimer1() TMR1IF = 0; TMR1ON = 1;
#define mStopTimer1() TMR1ON = 0; TMR1IF = 0

/** A D C **********************************************************/
// ADMF = 0 (left), Vref = Vdd, CHANEL = 0, ADON = OFF
#define mInitADC() ANSEL = 0b00000001; ADCON0 = 0b00000000
#define mStartADC() ADON = 1
#define mStopADC() ADON = 0
#define mRunADC() GO = 1

/** D E F I N E D **************************************************/
#define DELAY 400
#define IMPULS 200
#define MAX 1024*8
#define MIN 0
#define STEP 100

#endif	/* HEADER_H */
