#include <xc.h>
#include <pic18f4550.h>
#include <stdio.h>
#include "config.h"
#include "flex_lcd.h"
#define _XTAL_FREQ 8000000
void main(void) {
unsigned char i, j;
unsigned char buffer1[20];

  OSCCON= 0b01110010;        //configuración del Oscilador
  //*****************************************************

  ADCON1=0xff; //todo los puertos definidos como digitales

Lcd_Init();     //inicializamos el lcd
Lcd_Cmd(LCD_CLEAR);		//limpiamos lcd
Lcd_Cmd(LCD_CURSOR_OFF);	//apagamos el cursor
__delay_ms(100);		//esperamos 100ms

while(1){
    for(i=3; i>0; i--)
    {
        sprintf(buffer1,"Cuenta %03d",i);   //guardamos en el string Buffer1 la palabra Cuenta espacio y el valor de i
        Lcd_Out2(1, 1, buffer1);            //escribimos en el renglon uno espaco 1 la que contiene buffer1
        __delay_ms(1000);	//esperamos 100ms y repetimos
    }
    
    Lcd_Out(2, 1, "By luis Rivera");	
    __delay_ms(600);		//esperamos 600ms
    Lcd_Cmd(LCD_BLINK_CURSOR_ON);   //hacemos que parpade el cursor
    __delay_ms(1000);		//Esperamos 1s
    Lcd_Cmd(LCD_UNDERLINE_ON);	//subraya el cursor
    __delay_ms(1000);		//esperamos 1s
    Lcd_Cmd(LCD_CURSOR_OFF);	//apagamos el cursor
    __delay_ms(1000);
    Lcd_Cmd(LCD_CLEAR);		//limpiamos el cursor
    
    Lcd_Out(1, 4, "FUNCIONA");	//imprimimos hola
    Lcd_Out(2, 4, "yes");	//imprimimos gracias
    __delay_ms(2000);

    }
} 
