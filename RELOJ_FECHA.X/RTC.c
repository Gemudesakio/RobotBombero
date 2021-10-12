#include <xc.h>
#include <pic18f4550.h>
#include <stdio.h>
#include "config.h"
#include "LCD_Librery.h"
#include "I2C_librery.h"
#include <stdint.h>                        // Incluimos la librería stdint.

#define Pulsador_1  (PORTAbits.RA0)        // Pulsador 1 será asignado al pin RB0.
#define Pulsador_2  (PORTAbits.RA1)        // Pulsador 2 será asignado al pin RB1.

/*==========================================================================================================
 ===========================================================================================================*/
uint8_t hours;                             // Variable de 8 bits para las horas.
uint8_t minutes;                           // Variable de 8 bits para los minutos.
uint8_t seconds;                           // Variable de 8 bits para los segundos.
uint8_t year;                              // Variable de 8 bits para los año.
uint8_t month;                             // Variable de 8 bits para el mes.
uint8_t day;                               // Variable de 8 bits para el dia.
uint8_t i;                                 // Variable de 8 bits.

/*==========================================================================================================
 ===========================================================================================================*/
__bit Anti_rebote (void);                  // Función antirebote Pulsador_1 (RB0).
uint8_t Decimal_a_BCD (uint8_t numero);    // Función que convierte un número decimal a BCD. 
uint8_t BCD_a_Decimal (uint8_t numero);    // Función que convierte un número BCD a decimal.
void Reloj_Calendario (void);              // Función de visualización de HORA Y FECHA.
void Parpadeo (void);                      // Fucnción de parpadeo.
uint8_t Establecer_Hora_Fecha(uint8_t x, uint8_t y, uint8_t parametro);// Función para establecer HORA y FECHA.

/*==========================================================================================================
 ===========================================================================================================*/
__bit Anti_rebote (void)                   // Función antirebote Pulsador_1 (RB0).
{
    uint8_t contador = 0;                  // Variable tipo entero de 8 bits.
    for(uint8_t i=0;i<5;i++)               // Bucle for para generar como máximo 50ms de retardo.
    {
        if(Pulsador_1 == 0)                // Si Pulsador_1 fue presionado.
        contador++;                        // Incrementamos en una unidad la variable "contador".
        __delay_ms(10);                    // Retardo de 10 ms.
    }
    if(contador>2)                         // Si "contador" es mayor a 2.  
    {
        return 1;                          // Retornamos 1.
    }                       
    else                                   // De lo contrario.
    {
        return 0;                          // Retornamos 0.
    }
}   

/*==========================================================================================================
 ===========================================================================================================*/
uint8_t BCD_a_Decimal (uint8_t numero)            // Función que convierte un número BCD a decimal.
{
  return ((numero >> 4) * 10 + (numero & 0x0F));  // Retornamos la variable "numero" desplazado 4 posiciones a la izquierda, multiplicado por 10 más "numero" &&  1111.
}

/*==========================================================================================================
 ===========================================================================================================*/
uint8_t Decimal_a_BCD (uint8_t numero)            // Función que convierte un número decimal a BCD. 
{
    return (((numero / 10) << 4) + (numero % 10));// Retornamos la decena de la variable "numero" desplazado 4 bits a la derecha y las unidades de la variable "numero" en el nibble menos significativo
}

/*==========================================================================================================
 ===========================================================================================================*/
void Reloj_Calendario (void)                //Función de visualización de HORA Y FECHA.
{
    static char Time[] = "TIME: 00:00:00";  // Arreglo estático char "Time". 
    static char Date[] = "DATE: 00/00/2000";// Arreglo estático char "Date".
  
    seconds = BCD_a_Decimal(seconds);       // Llamamos a la función de conversión de BCD a Decimal.
    minutes = BCD_a_Decimal(minutes);       // Llamamos a la función de conversión de BCD a Decimal.
    hours   = BCD_a_Decimal(hours);         // Llamamos a la función de conversión de BCD a Decimal.
    day     = BCD_a_Decimal(day);           // Llamamos a la función de conversión de BCD a Decimal.
    month   = BCD_a_Decimal(month);         // Llamamos a la función de conversión de BCD a Decimal.
    year    = BCD_a_Decimal(year);          // Llamamos a la función de conversión de BCD a Decimal.
    
    Time[6] =  hours   / 10 + '0';          // Guardamos las decenas de la variable "hours" en la posición 6 del arreglo "Time".
    Time[7] =  hours   % 10 + '0';          // Guardamos las unidades de la variable "hours" en la posición 7 del arreglo "Time".
    Time[9] =  minutes / 10 + '0';          // Guardamos las decenas de la variable "minutes" en la posición 9 del arreglo "Time".
    Time[10]=  minutes % 10 + '0';          // Guardamos las unidades de la variable "minutes" en la posición 10 del arreglo "Time".
    Time[12]=  seconds / 10 + '0';          // Guardamos las decenas de la variable "seconds" en la posición 12 del arreglo "Time".
    Time[13]=  seconds % 10 + '0';          // Guardamos las unidades de la variable "secods" en la posición 13 del arreglo "Time".
    
    Date[6] =  day     / 10 + '0';          // Guardamos las decenas de la variable "day" en la posición 6 del arreglo "Date".
    Date[7] =  day     % 10 + '0';          // Guardamos las unidades de la variable "day" en la posición 7 del arreglo "Date".
    Date[9] =  month   / 10 + '0';          // Guardamos las decenas de la variable "month" en la posición 9 del arreglo "Date".
    Date[10]=  month   % 10 + '0';          // Guardamos las unidades de la variable "month" en la posición 10 del arreglo "Date".
    Date[14]=  year    / 10 + '0';          // Guardamos las decenas de la variable "year" en la posición 14 del arreglo "Date".
    Date[15]=  year    % 10 + '0';          // Guardamos las unidades de la variable "year" en la posición 15 del arreglo "Date".
    
    LCD_Goto(1, 1);                         // Cursor en fila 1, columna 1.
    LCD_Print(Time);                        // Imprimimos en la pantalla LCD el valor del arreglo "Time".
    LCD_Goto(1, 2);                         // Cursor en fila 2, columna 1.
    LCD_Print(Date);                        // Imprimimos en pantalla LCD el valor del arreglo "Date". 
}

/*==========================================================================================================
 ===========================================================================================================*/
void Parpadeo (void)                        // Función Parpadeo().
{
    uint8_t j=0;                            // Variable entera de 8 bits.
    while(j<100 && Pulsador_1 && Pulsador_2)// Mientras j sea menor que 100 y Pulsador_1 y Pulsador_2 sean igual a 1.
    {
        j++;                                // Incrementamos el valor de j en una unidad.
        __delay_ms(5);                      // 5 milisegundos.
    }
}

/*==========================================================================================================
 ===========================================================================================================*/
uint8_t Establecer_Hora_Fecha(uint8_t x, uint8_t y, uint8_t parametro) // Función para establecer Hora y Fecha.
{
    while(Anti_rebote());                   // Mientras la  función Anti_rebote() devuelva un 1.
    while(1)                                // Bucle infinito.
    {
        while(!Pulsador_2)                  // Mientras Pulsador_2 este presionado. pin RB1.  
        {
            parametro++;                    // Incrementamos en una unidad el valor de la variable "parametro".
            if(i==0 && parametro>23)        // Si i=0 y "parametro" es mayor a 23 (horas).
            {
                parametro=0;                // parametro = 0.
            }
            if(i==1 && parametro>59)        // Si i=1 y "parametro" es mayor a 59 (minutos).
            {
                parametro=0;                // parametro = 0.
            }
            if(i==2 && parametro>31)        // Si i=2 y "parametro" es mayor a 31 (dias).
            {
                parametro=1;                // parametro = 1.
            }
            if(i==3 && parametro>12)        // Si i=3 y parametro es mayor a 12 (meses).
            {
                parametro=1;                // parametro = 1.
            }
            if(i==4 && parametro>99)        // Si i=4 y "parametro" es mayor a 99 (año).
            {
                parametro=0;                // parametro = 0.
            }
            LCD_Goto(x,y);                  // Cursos de la pantalla LCD en columna "x", fila "y".
            LCD_PutC(parametro / 10 + '0'); // Imprimimos en la pantalla LCD las decenas de la variable "parametro". 
            LCD_PutC(parametro % 10 + '0'); // Imprimimos en la pantala LCD las unidades de la variable "parametro".
            __delay_ms(200);                // Retardo de 200 ms.
        }
        
        LCD_Goto(x, y);                     // Cursos de la pantalla LCD en columna "x", fila "y".                    
        LCD_Print("  ");                    // Inprimimos en la pantalla LCD 2 espacios vacios.
        Parpadeo();                         // Llamamos a la función Parpadeo().
        LCD_Goto(x, y);                     // Cursos de la pantalla LCD en columna "x", fila "y".
        LCD_PutC(parametro / 10 + '0');     // Imprimimos en la pantalla LCD las decenas de la variable "parametro". 
        LCD_PutC(parametro % 10 + '0');     // Imprimimos en la pantalla LCD las unidades de la variable "parametro". 
        Parpadeo();                         // Llamamos a la función Parpadeo().
        
        if(!Pulsador_1)                     // Si el Pulsador_1 es presionado.
        if(Anti_rebote())                   // Si la función Anti_rebote() devuelve un 1.
          {
            i++;                            // Incrementamos el valor de i en una unidad.
            return parametro;               // Retornamos el valor de la variable "parametro".
          }
    }
}

/*==========================================================================================================
 ===========================================================================================================*/
void main(void)                             // Función Principal.
{
    OSCCON = 0b01110010; //configuracion del oscilador
    ADCON1=0b00001111;                      // Deshabilitamos las entradas analógicas del puerto A y B.
    TRISA=0b11;                             // Configuramos los pines RA0 y RA1 como entradas digitales.
    I2C_Init(100000);                       // Inicializamos la comunicación I2C con una frecuencia de 100KHz. para el DS1307.
    LCD_Begin();                            // Inicializamos la pantalla LCD 16x2.
    
    while(1)                                // Bucle infinito.
    {
        if(!Pulsador_1)                     // Si Pulsador_1 es presionado.
        if(Anti_rebote())                   // Si la función Anti_rebote() devuelve un 1.
            {
                i=0;                        // La variable i = 0.          
                hours  = Establecer_Hora_Fecha(7,1,hours);   //  Llamamos a la función Establecer_Hora_Fecha con x=7; y=1; parametro=hours.
                minutes= Establecer_Hora_Fecha(10,1,minutes);//  Llamamos a la función Establecer_Hora_Fecha con x=10; y=1; parametro=minutes.
                day    = Establecer_Hora_Fecha(7,2,day);     //  Llamamos a la función Establecer_Hora_Fecha con x=7; y=2; parametro=day.
                month  = Establecer_Hora_Fecha(10,2,month);  //  Llamamos a la función Establecer_Hora_Fecha con x=10; y=2; parametro=month.
                year   = Establecer_Hora_Fecha(15,2,year);   //  Llamamos a la función Establecer_Hora_Fecha con x=15; y=2; parametro=year..
                while(Anti_rebote());                        // Esperamos mientras Anti_rebote() sea 1.
                minutes= Decimal_a_BCD(minutes);             // Llamamos a la función Decimal_a_BCD(numero=minutes).
                hours  = Decimal_a_BCD(hours);               // Llamamos a la función Decimal_a_BCD(numero=hours).
                day    = Decimal_a_BCD(day);                 // Llamamos a la función Decimal_a_BCD(numero=day).
                month  = Decimal_a_BCD(month);               // Llamamos a la función Decimal_a_BCD(numero=mes). 
                year   = Decimal_a_BCD(year);                // Llamamos a la función Decimal_a_BCD(numero=año).
                
                I2C_Start();                // Llamamos a la función Start.
                I2C_Write(0xD0);            // Escribimos en SSPBUF la dirección de DS1307 1101000 + 0 de escritura.
                I2C_Write(0x00);            // Dirección de segundos.
                I2C_Write(0x00);            // Reiniciamos los segundos.
                I2C_Write(minutes);         // Cargamos el valor de minutos en la dirección de minutos.
                I2C_Write(hours);           // Cargamos el valor de horas en la dirección de horas.
                I2C_Write(1);               // No utilizamos la dirección 0x03.
                I2C_Write(day);             // Cargamos el valor de dias en la dirección  de 0x04.
                I2C_Write(month);           // Cargamos el valor de mes en la dirección de mes.
                I2C_Write(year);            // Cargamos el valor de año en la dirección de años.
                I2C_Stop();                 // Llamamos a la función Stop.
                __delay_ms(200);            // Retardo de 200 ms.
            }
        
        I2C_Start();                        // Llamamos a la función Start.
        I2C_Write(0xD0);                    // Escribimos en SSPBUF la dirección de DS1307 1101000 + 0 de escritura.
        I2C_Write(0);                       // Dirección de segundos.
        I2C_ReStart();                      // Llamamos a la función ReStart.
        I2C_Write(0xD1);                    // Escribimos en SSPBUF la dirección de DS1307 1101000 +1 de lectura.
        seconds=I2C_Read();                 // Cargamos la variable "seconds" con el valor de SSPBUF.
        I2C_Ack();                          // ACK.
        minutes=I2C_Read();                 // Cargamos la variable "minutes" con el valor de SSPBUF.
        I2C_Ack();                          // ACK.
        hours=I2C_Read();                   // Cargamos la variable "hours" con el valor de SSPBUF.
        I2C_Ack();                          // ACK.
        I2C_Read();                         // Valor no leido.
        I2C_Ack();                          // ACK.
        day=I2C_Read();                     // Cargamos la variable "day" con el valor de SSPBUF.
        I2C_Ack();                          // ACK.
        month=I2C_Read();                   // Cargamos la variable "month" con el valor de SSPBUF.
        I2C_Ack();                          // ACK.
        year=I2C_Read();                    // Cargamos la variable "year" con el valor de SSPBUF.
        I2C_NO_Ack();                       // NO ACK.
        I2C_Stop();                         // Llamamos a la función Stop.
        
        Reloj_Calendario();                 // Llamamos a la función Reloj_Calendario().
        __delay_ms(50);                     // Retardo de 50 ms.
    }
}