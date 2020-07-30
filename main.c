/*
 * ICU.c
 *
 * Created: 7/30/2020 7:43:38 PM
 * Author : BISHOY
 */ 

#include "Timer.h"
#include "LCD.h"
#define F_CPU 16000000
#include <util/delay.h>

extern volatile uint8  ICU_Counter;
extern volatile uint32 C1;
extern volatile uint32 C2;
extern volatile uint32 C3;

int main(void)
{
	uint32 TON=0;
	uint32 TOTAL=0;
	uint32 Duty_Cycle=0;
	uint32 Frequency=0;
	
	LCD_Init();
	timer1_init();
	timer2_init();
	PWM2_Value(75);
	timer2_start();
	DIO_SetPinDir(DIO_PORTD , DIO_PIN7 ,DIO_PIN_OUTPUT);
	ICU_Init ();
	timer1_start();
    while (1) 
    {
		if(ICU_Counter==3)
		{
			TON=C2-C1;
			TOTAL=C3-C1;
			Duty_Cycle=(TON*100)/TOTAL;
			Frequency=16000000 /(1 * TOTAL);
			
			LCD_WriteString("DUTY CYCLE:");
			LCD_WriteInteger(Duty_Cycle);
			LCD_GoTo(1, 0);
			LCD_WriteString("Frequency:");
			LCD_WriteInteger(Frequency);
			_delay_ms(500);
			LCD_Clear();
			SET_BIT(TIMSK,5);
			SET_BIT(TIMSK,2);
		}
    }
	
}

