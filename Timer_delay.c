#include <xc.h> //XC16 compiler definitions
#include <p24F16KA101.h>
#include "Timer_delay.h"

//uint16_t time; // time should be in s this is just global variable and does nothing

volatile uint8_t t2Done;  // keeping t2Done = 0 also works 
//volatile uint8_t t1Done;  // keeping t2Done = 0 also works

void TimerInit(void) {
    //timer2 configuration

    T2CONbits.TCS = 0; // use internal clock
    
    IPC1bits.T2IP = 2; // priority bits
    IFS0bits.T2IF = 0; // interrupt request has not occurred yet
    IEC0bits.T2IE = 1; // interrupt request enabled
   
    
    //timer1 configuration
    T1CONbits.TCS = 0;
    IPC0bits.T1IP = 5; // higher priority than timer 2
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CONbits.TCKPS = 1;
    PR1 = 7811;
    TMR1=0;
    T1CONbits.TON = 1;
    
    
}


/*void delay1_s(void){
    
    T1CONbits.TCKPS = 1;
    PR1 = 7811;
    TMR1=0;
    t1Done = 0;
    while(!t1Done) {
        Idle();
        
    }
            
}*/

void delay_s(float time){  
    uint16_t prescaler = (((time)*125000)/65536) ; // prescaler ticks  
    if (prescaler <= 1){        // prescaler 1:1             
        PR2 = (time * 125000)-1 ;
        T2CONbits.TCKPS = 0; // actually set the prescaler to 1
    }
    else if (1 < prescaler && prescaler <= 8) {
        PR2 = ((time * 125000) / 8)-1 ; // prescaler 1:8
        T2CONbits.TCKPS = 1;
    }
    else if (8 < prescaler && prescaler <= 64) {
        PR2 = ((time * 125000) / 64)-1 ;
        T2CONbits.TCKPS = 2;
    }
    else if (64 < prescaler && prescaler <= 256) {
        PR2 = ((time * 125000) / 256)-1 ;
        T2CONbits.TCKPS = 3;
    }
    
    
    TMR2=0; // always set TMR to 0 first 
    t2Done = 0; // once the timer goes off, t2Done sets to 1. we manually have to clear it
    T2CONbits.TON = 1; // then start the timer
    while(!t2Done) {
        Idle();
    }
}

// Timer1 ISR: toggle LED2 every 0.5 s
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0; // clear flag
    //T1CONbits.TON = 0; //  Why don't we turn off the timer like timer 2?
    LATAbits.LATA6 ^= 1; //  XOR bit mask ( if turn on -> turn off vice versa)
}


// Timer2 ISR: signals end of delay
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0; // clear flag
    T2CONbits.TON = 0; // turn off timer 2
    t2Done = 1;
}








