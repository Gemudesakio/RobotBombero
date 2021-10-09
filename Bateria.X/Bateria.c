#include <xc.h>
#include <pic18f4550.h>
#include <stdio.h>
#include "config.h"
#define _XTAL_FREQ 8000000

#define IMPUT 1
#define OUTPUT 0
#define CFG_BATERIA TRISA
void main(void){
    TRISD = 0x00; //puerto d como salida
    CFG_BATERIA = IMPUT; // configura puerto A como entrada 
    ADCON1bits.PCFG = 0xD; // an0 y and1 entradas analogicas y el resto digitales
    ADCON2bits.ADFM = 0; // justificacion a la izquierda
    ADCON1bits.VCFG0 = 0; // seleccion voltaje de referencia de 0v a 5v
    ADCON1bits.VCFG1 = 0;   // seleccion voltaje de referencia de 0v a 5v
    ADCON0bits.CHS = 0b0000; // selecciono entrada AN0
    ADCON2bits.ADCS = 0b001; // tiempom de conversion Fosc/8
    
    while(1){
       ADCON0bits.ADON = 1; //habilita el modulo AD
       __delay_us(30);
       ADCON0bits.GO = 1; //inicia la conersion AD
       while(ADCON0bits.GO); // espera que termine la conversion 
       ADCON0bits.ADON =0; //inhabilita la conversion AD
       PORTD = ADRESH; 
       __delay_us(500);
    }
}
