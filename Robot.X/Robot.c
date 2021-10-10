#include <xc.h>
#include <pic18f4550.h>
#include <stdio.h>
#include "config.h"
#include "lcd.h"
#define _XTAL_FREQ 8000000

#define IMPUT 1
#define OUTPUT 0

#define ON 1
#define OFF 0

#define CFG_LED_WHITE TRISC0
#define LED_WHITE RC0
 
#define CFG_SENSOR_HUMO TRISA2
#define SENSOR_HUMO RA2

short getCad (char canal){
    ADCON1 = 0b00001101; // convierte en analogicos los canales an0, an1, an2
        switch(canal){
          case 0: 
                ADCON0 = 0b00000010; // inicia la conversion A/D del canal an0    
                break;
         }
        ADCON2 = 0b00101001; // configuracion de velocidad de toam de datos del dac
        ADON = 1;
        GO_DONE = 1; // Conversion A/D en marcha
        
        return ADRESH; //Drvuelve los datos de la convetsion A/D
}
void main(void){
    CFG_LED_WHITE = OUTPUT;
    CFG_SENSOR_HUMO = IMPUT;
    OSCCON = 0b01110010; //configuracion del oscilador
    float temperatura;
    
    TRISA = 0xff; // puerto A como entrada
    TRISD = 0; // puerto D como salida
    TRISB = 0x00; // puerto B como salida
    char temp[20];
    
    Lcd_Init();  //inicializamos el lcd
    Lcd_Cmd(LCD_CLEAR); //limpiamos la lcd
    Lcd_Cmd(LCD_CURSOR_OFF); // apagamos el cursor
    while(1){
        #asm
       
        #endasm
        temperatura = (getCad(0)*0.02*100); // convierte el valor leido al real
        sprintf(temp, "%2.2f C", temperatura);
        Lcd_Out2(1, 4, temp);
        __delay_ms(200);
        if(temperatura>100){
            Lcd_Out2(2, 4, "T Alta");
        }
        else{
            if(temperatura>30){
                Lcd_Out2(2, 4, "T Media");
            }
            else{
                Lcd_Out2(2, 4, "T Baja");
            }
        }
           if(SENSOR_HUMO == 1){
            LED_WHITE = ON;
        }
        else{
            LED_WHITE = OFF;
    }
    }
}