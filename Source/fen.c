/*
 * File:   fen.c
 * Author: nva1773@gmail.com
 *
 * Created on 12/10/2020
 * "Created in MPLAB X IDE"
 */

// Configuration Bit Settings
#pragma config FOSC = INTRCIO // Oscillator Selection bits (INTRC oscillator, GP4 and GP5 I/O)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enable, 18ms without prescaler)
#pragma config PWRTE = ON // Power-Up Timer Enable bit (PWRT enable = 72ms)
#pragma config MCLRE = ON // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = ON // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF // Data Code Protection bit (Data memory code protection is disabled)

// Files include
#define _XTAL_FREQ 4000000  // for debuger
#include <xc.h>             // access to SFRs
//#include <stdint.h>       // for uint8_t, int16_t ...
#include <pic12f675.h>
#include "header.h"
#include "typedef.h"

// Prototypes
void InitCPU(void);
void InitVAR(void);
void Control(void);

// Bits Variable: __bits -> one byte (0b7...0)
//__bit isFlag_b7, isFlag_b6, isFlag_b5, isFlag_b4, isFlag_b3,  isFlag_b2, isFlag_b1, isFlag_b0;
// used:
// isFlag_b0 = 1;
// isFlag_b1 = 1;
// OR
// BITS Flag1;
// used:
// Flag1.byte = 0xFF;
// Flag1.bits.b0 = false;
// Flag1.bits.b2 = false;

// Variable
__bit isFirst, isDelay10ms;
unsigned int duration, adjust;

// Main function
void main(void)
{
    OSCCAL = __osccal_val();
    InitCPU();
    InitVAR();
    
    mInterruptEnable();
    
    while(1)
    {
        if(COUT == 1)
        {
            mStopTimer0();
            mLoadTimer0();
            mStartTimer0();
            isDelay10ms = true;
            Control();
            while(isDelay10ms) continue;
            Control();
            isFirst = true;
        }
    }
    
    return;
}

// Initialization CPU
void InitCPU(void)
{
    mInterruptDisable();
    mInitPorts();
    mInitTimer0();
	mInitTimer1();
    mInitADC();
}

// Initialization Variable
void InitVAR(void)
{
    isFirst = false; isDelay10ms = false;
    duration = MAX;
}

// Contorl for triac
void Control(void)
{
 	// Read ADC0 -> 0 = Min...8192 = Max
    mStartADC();
    __delay_us(DELAY);
    mRunADC();
    while(GO) continue;
    adjust = ADRESH; //0...256
    duration = MAX - (adjust << 5);
    
    // Out control
    if(isFirst & isEnable)
    {
        mLoadTimer1(duration);
        mStartTimer1();
        while(!TMR1IF) continue;
        mStopTimer1();
        mStartOUT();
        __delay_us(IMPULS);
    }
    mStopOUT();
} 

// Interrupt
void __interrupt() myISR(void)
{
	// For TMR0 -> 10ms 
	if(mInterruptTMR0)
	{
	    mStopTimer0();
        mLoadTimer0();
        isDelay10ms = false;
	}
}
