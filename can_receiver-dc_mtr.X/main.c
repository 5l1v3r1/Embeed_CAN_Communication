/*
 * File:   main.c
 * Author: krishna
 *
 * Created on 29 November, 2017, 10:58 PM
 */




// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "lcd.h"
#include "can.h"

#define _XTAL_FREQ 8000000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#byte p_d = 0xf83 
#byte PIR1 = 0xf9e 
int8 count; 

#int_timer2 
void isr_timer2(void) { 
bit_clear(PIR1,1); 
   count++; //keep a running timer that increments every milli-second 
}
int main()
{
    unsigned int a;
    unsigned int data;
    short duty = 0; //initial value for dutyTRISD = 0xFF; //PORTD as input
    TRISC = 0x00; //PORTC as output
    TRISB = 0x00; //PORTB as output
    PORTB = 0x02; //Run motor in anticlock wise
    TRISD = 0x00;
   
    Lcd_Init();

    PWM1_Init(1000); //Initialize PWM1
    PWM1_Start(); //start PWM1
    PWM1_Set_Duty(duty); //Set current duty for PWM1
    
    //can protocol
    
    struct rx_stat rxstat; //??? ??? ???? 
   int32 rx_id; 
   int in_data[8]; 
   int rx_len; 

//send a request (tx_rtr=1) for 8 bytes of data (tx_len=8) from id 24 (tx_id=24) 
   int out_data[8]; 
   int32 tx_id=24; 
   int1 tx_rtr=1; 
   int1 tx_ext=0; 
   int tx_len=8; 
   int tx_pri=3; 

   int i; 

   for (i=0;i<8;i++) { 
      out_data[i]=0; 
      in_data[i]=0; 
   } 

   printf("\r\n\r\nCCS CAN EXAMPLE\r\n"); 

   setup_timer_2(T2_DIV_BY_16,255,16);   //1??? ??. 

   can_init();//Initializes PIC18xxx8 CAN peripheral. 

   enable_interrupts(INT_TIMER2);   //enable timer2 interrupt 
   enable_interrupts(GLOBAL);       //enable all interrupts (else timer2 wont happen) 

   printf("\r\nRunning..."); 

   while(TRUE) 
   { 
//Data receive 
      if ( can_kbhit() )   //if data is waiting in buffer... 
      { 
         if(can_getd(rx_id, &in_data[0], rx_len, rxstat)) { //...then get data from buffer 
            printf("\r\nGOT: BUFF=%U ID=%LU LEN=%U OVF=%U ", rxstat.buffer, rx_id, rx_len, rxstat.err_ovfl); 
            printf("FILT=%U RTR=%U EXT=%U INV=%U", rxstat.filthit, rxstat.rtr, rxstat.ext, rxstat.inv); 
            printf("\r\n    DATA = "); 
            for (i=0;i<rx_len;i++) { 
               printf("%X ",in_data[i]); 
            } 
            printf("\r\n"); 
            data=rx_len;
         } 
         else { 
            printf("\r\nFAIL on GETD\r\n"); 
         } 

      
    // dc motor pwm part
    if (data > 10)
    // endless loop
    {
    if (!RD0_bit && duty<260) //if button on RD0 pressed
    {
    __delay_ms(40);
    duty = duty + 10; 
    PWM1_Set_Duty(duty); 
    }
    if (!RD1_bit && duty >0) 
    {
    __delay_ms(40);
    duty = duty - 10;
    PWM1_Set_Duty(duty);
    }
    __delay_ms(10);

    //lcd code part
    while(1)
    {
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("received value");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(data);
        __delay_ms(2000);
        Lcd_Clear();
    }
    }
    return 0;
}
   }