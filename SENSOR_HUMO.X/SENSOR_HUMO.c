#include <xc.h>
#include <pic18f4550.h>
#include <stdio.h>
#include "config.h"
#define _XTAL_FREQ 8000000

#define IMPUT 1
#define OUTPUT 0

#define ON 1
#define OFF 0

#define CFG_LED_WHITE TRISC0
#define LED_WHITE RC0
 
#define CFG_SENSOR_HUMO TRISA2
#define SENSOR_HUMO RA2



void main (void){
    ADCON1bits.PCFG = 0xD; 
    CFG_LED_WHITE = OUTPUT;
    CFG_SENSOR_HUMO = IMPUT;
    while(1){
       
         if(SENSOR_HUMO == 1){
            LED_WHITE = ON;
        }
        else{
            LED_WHITE = OFF;
    }
    } 
    
}
