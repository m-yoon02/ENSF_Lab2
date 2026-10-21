#include <xc.h> //XC16 compiler definitions
#include <p24F16KA101.h>
#include "clkChange.h"
#include "Timer_delay.h"




#define PB1 (PORTBbits.RB7 == 0)
#define PB2 (PORTBbits.RB4 == 0)
#define PB3 (PORTAbits.RA4 == 0)
#define LED1 (LATBbits.LATB9)
#define LED2 (LATAbits.LATA6)

//initialize io ports and io checkes/LED blinking

void IOinit(void) {
    AD1PCFG = 0xFFFF; 
    //pin direction
    //PB1: RB7, Pb2: RB4, PB3: RA4, LED1: RB9, LED2: RA6
    TRISBbits.TRISB7 = 1; //PB1 input
    TRISBbits.TRISB4 = 1; // PB2 input
    TRISAbits.TRISA4 = 1; // PB3 input 
    TRISBbits.TRISB9 = 0; // LED1 output 
    TRISAbits.TRISA6 = 0; // LED2 output
    
    //pull up (active low))
    CNPU2bits.CN23PUE = 1;//PB1(RB7)
    CNPU1bits.CN1PUE = 1;//PB2(RB4)
    CNPU1bits.CN0PUE = 1;//PB3(RA4)
    
    LATBbits.LATB9 =0;
    LATAbits.LATA6 =0;
}

void IOCheck(void) {
    //add button implementation and usage
    if (PB1) {
        LED1 =1; //LED1 on
        delay_s(0.5);
        LED1 =0; //LED1 off
        delay_s(0.5);
    }
}
