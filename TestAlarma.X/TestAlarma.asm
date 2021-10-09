; PIC18F4550 Configuration Bit Settings

; ASM source line config statements

#include "p18F4550.inc"

; CONFIG1L
  CONFIG  PLLDIV = 1            ; PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
  CONFIG  CPUDIV = OSC1_PLL2    ; System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
  CONFIG  USBDIV = 1            ; USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

; CONFIG1H
  CONFIG  FOSC = INTOSC_HS      ; Oscillator Selection bits (Internal oscillator, HS oscillator used by USB (INTHS))
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
  CONFIG  IESO = OFF            ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

; CONFIG2L
  CONFIG  PWRT = OFF            ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  BOR = ON              ; Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
  CONFIG  BORV = 3              ; Brown-out Reset Voltage bits (Minimum setting 2.05V)
  CONFIG  VREGEN = OFF          ; USB Voltage Regulator Enable bit (USB voltage regulator disabled)

; CONFIG2H
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  WDTPS = 32768         ; Watchdog Timer Postscale Select bits (1:32768)

; CONFIG3H
  CONFIG  CCP2MX = ON           ; CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
  CONFIG  PBADEN = OFF          ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
  CONFIG  LPT1OSC = OFF         ; Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
  CONFIG  MCLRE = ON            ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

; CONFIG4L
  CONFIG  STVREN = ON           ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
  CONFIG  LVP = ON              ; Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
  CONFIG  ICPRT = OFF           ; Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
  CONFIG  XINST = OFF           ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; CONFIG5L
  CONFIG  CP0 = OFF             ; Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
  CONFIG  CP1 = OFF             ; Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
  CONFIG  CP2 = OFF             ; Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
  CONFIG  CP3 = OFF             ; Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

; CONFIG5H
  CONFIG  CPB = OFF             ; Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
  CONFIG  CPD = OFF             ; Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

; CONFIG6L
  CONFIG  WRT0 = OFF            ; Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
  CONFIG  WRT1 = OFF            ; Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
  CONFIG  WRT2 = OFF            ; Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
  CONFIG  WRT3 = OFF            ; Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

; CONFIG6H
  CONFIG  WRTC = OFF            ; Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
  CONFIG  WRTB = OFF            ; Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
  CONFIG  WRTD = OFF            ; Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

; CONFIG7L
  CONFIG  EBTR0 = OFF           ; Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
  CONFIG  EBTR1 = OFF           ; Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
  CONFIG  EBTR2 = OFF           ; Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
  CONFIG  EBTR3 = OFF           ; Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

; CONFIG7H
  CONFIG  EBTRB = OFF           ; Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

list p=18f4550  ;nombres de los registros que vamos a utilizar
     TEMP equ 0x20 ; registro de memoria F
     CHIGH equ .100 ; valor alto de la temperatura 
     CLOW equ .25 ; valor promedio de temperatura ambiente    
    org 0x00
    goto INIT
INIT
   
    ;Inicio de programa
   MOVLW   0xF	   ; valor dado por el datasheet para acceder al banco 15
   MOVWF   BSR     ; aceso al banco 15 donde estan las sfr 
   MOVLW   .10    ; Valor par iinicializar la variable
   MOVWF   TEMP	   ; cargamos el valor en la variable
   MOVF    TEMP,0  ; Almaceno el valor de la variable en el regisdtro intermediario W
 
   ;Configuracion puertos analogos y comparadores
   MOVLW   0x0E    ; AN0 como entrada analogica, las demas como digitales 
   MOVWF   ADCON1  ; para puertos digitales
   MOVLW   0x3     ; valor usado para inicializar datos, direccion
   MOVLW   0x7     ; Apagar
   MOVWF   CMCON   ; Comparadores
  
   ;inicializacion del puerto E
   CLRF    PORTE   ; inicializa el puerto e limpiando todas sus salidas
   CLRF    LATE    ; forma alternativa de limpiar las salidas
   MOVLW   0x88    ; valor de literal para fijar en TRISE
   MOVWF   TRISE   ; Set RE<2:0> as outputs
  
VERIFICAR_VALORMAX
   MOVF  TEMP,0  ; Almaceno el valor de la variable en el regisdtro intermediario W
   SUBLW CHIGH ; Resta que compara si el valor a entra es mayor, menor o igual a la constante mas alta
   BTFSC STATUS,0 ; revisa el registro del bit de carry, si es 1 el resultado de l resta es positivo y si es 0 el resultado es negativo
   GOTO  Verificar_25
   GOTO  LED_ROJO ; enciende el led rojo, si la temperatura es mayor que 100
   
   Verificar_25
	MOVF    TEMP,0  ; Almaceno el valor de la variable en el regisdtro intermediario W
	SUBLW CLOW ; Resta que compara si el valor a entra es mayor, menor o igual a la constante mas alta
        BTFSC STATUS,0 ; revisa el registro del bit de carry, si es 1 el resultado de l resta es positivo y si es 0 el resultado es negativo
	GOTO LED_VERDE ; enciende el led verde, si la temperatura es menor que 25
	GOTO LED_AMARILLO ; enciende el led amarillo, si la temperatura es mayor que 25 pero menor que 10
   LED_ROJO
	BSF PORTE,2
	BCF PORTE,1
	BCF PORTE,0
	GOTO INC_TEMP
   LED_AMARILLO
	BSF PORTE,1
	BCF PORTE,2
	BCF PORTE,0
	GOTO INC_TEMP
   LED_VERDE
	BSF PORTE,0
	BCF PORTE,1
	BCF PORTE,2
	GOTO INC_TEMP
   INC_TEMP
       GOTO VERIFICAR_VALORMAX
       
end
    



