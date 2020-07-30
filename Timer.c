/*
 * Timer.c
 *
 * Created: 7/17/2020 7:12:18 PM
 *  Author: 20112
 */ 
#include "Timer.h"
#include "Timer_Cfg.h"
#include <avr/interrupt.h>

/* *********************************************************************** TIMER0_Variables ****************************************************************** */

uint32 Timer0_over_flow=0;
uint8  init_value=0;
uint32 Timer0_Compare_Match=0;
volatile uint8 count0=0;
static void (*Timer0_Ptr)(void);

/* *********************************************************************** TIMER1_Variables ****************************************************************** */
volatile uint32 Timer1_over_flow=0;
volatile uint8  ICU_Counter=0;
volatile uint32 C1=0;
volatile uint32 C2=0;
volatile uint32 C3=0;

/* *********************************************************************** TIMER2_Variables ****************************************************************** */

uint32 Timer2_over_flow=0;
uint8  init_value2=0;
uint32 Timer2_Compare_Match=0;
volatile uint8 count2=0;
static void (*Timer2_Ptr)(void);


/* ***************************************************************************** TIMER0 ********************************************************************* */

void timer0_init(void)
{
	CLR_BIT(TCCR0,7);
	
	#if     Timer0_Mode    ==       TIMER_Normal_Mode
	{
		CLR_BIT(TCCR0,6);
		CLR_BIT(TCCR0,3);
		
		#if interrupt0_state  ==    interrupt_Enable 
		{
			SET_BIT(SREG ,7);
			SET_BIT(TIMSK,0);
		}
		#endif
	}
	#elif   Timer0_Mode    ==       TIMER_CTC_Mode
	{
		CLR_BIT(TCCR0,6);
		SET_BIT(TCCR0,3);
		
		#if interrupt0_state  ==    interrupt_Enable
		{
			SET_BIT(SREG ,7);
			SET_BIT(TIMSK,1);
		}
		#endif
	}
	
	#elif   Timer0_Mode    ==       TIMER_Fast_PWM_Mode 
	{
		SET_BIT(TCCR0,6);
		SET_BIT(TCCR0,3);
		
		#if   PWM0_Mode    ==   PWM_NON_Inverted_Mode
		{
			SET_BIT(TCCR0,5);
			CLR_BIT(TCCR0,4);
		}
		#elif PWM0_Mode    ==   PWM_Inverted_Mode
		{
			SET_BIT(TCCR0,5);
			SET_BIT(TCCR0,4);
		}
		#endif
	}
	#elif   Timer0_Mode    ==       TIMER_PWM_Phase_Correct_Mode
	{
		SET_BIT(TCCR0,6);
		CLR_BIT(TCCR0,3);
		
		#if   PWM0_Mode    ==   PWM_NON_Inverted_Mode
		{
			SET_BIT(TCCR0,5);
			CLR_BIT(TCCR0,4);
		}
		#elif PWM0_Mode    ==   PWM_Inverted_Mode
		{
			SET_BIT(TCCR0,5);
			SET_BIT(TCCR0,4);
		}
		#endif
	}
	#endif
	
}



void timer0_start(void)
{
	
	  #if  Timer0_CLK     ==       Pres_CLK   
  {
	  SET_BIT(TCCR0,0);
	  CLR_BIT(TCCR0,1);
	  CLR_BIT(TCCR0,2);
  } 
  #elif  Timer0_CLK     ==       Pres_CLK_8 
  {
	  CLR_BIT(TCCR0,0);
	  SET_BIT(TCCR0,1);
	  CLR_BIT(TCCR0,2);
  }
  #elif  Timer0_CLK     ==       Pres_CLK_64
  {
      SET_BIT(TCCR0,0);
      SET_BIT(TCCR0,1);
	  CLR_BIT(TCCR0,2);
  }
 #elif  Timer0_CLK     ==       Pres_CLK_256
 {
	 CLR_BIT(TCCR0,0);
	 CLR_BIT(TCCR0,1);
	 SET_BIT(TCCR0,2);
 } 
 #elif  Timer0_CLK     ==      Pres_CLK_1024
 {
	 SET_BIT(TCCR0,0);
	 CLR_BIT(TCCR0,1);
	 SET_BIT(TCCR0,2);
 }
 #endif
 
}

void timer0_stop(void)
{
	CLR_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	CLR_BIT(TCCR0,2);
	
}


void timer0_delay(uint32 delay_ms )
{
	#if Timer0_Mode     ==      TIMER_Normal_Mode
	{		
	  uint8 tick_time = Timer0_CLK /Freq;    // micro second
		
	  uint32 num_tick = (delay_ms*1000)/tick_time;
		
	  Timer0_over_flow = num_tick/256;
		
	  init_value = 256 -(   (  (float32) num_tick/256 - Timer0_over_flow  ) * 256  );
		
	  TCNT0 = init_value;
	
	  Timer0_over_flow++;
	}
	
	#elif Timer0_Mode     ==      TIMER_CTC_Mode 
	{
		 uint8 tick_time = Timer0_CLK /Freq;    // micro second
		 
		 uint32 num_tick = (delay_ms*1000)/tick_time;
		 
		 Timer0_Compare_Match = num_tick/255;
		 
		 init_value =    ((  (float32) num_tick/255 - Timer0_Compare_Match  ) * 255)-1;
		 
		 OCR0 = init_value;
		 
		 Timer0_Compare_Match++;
	}
	#endif
}

void timer0_Set_CallBack(void (*ptr)(void))
{
	Timer0_Ptr = ptr;
}

/* ************************************************ PWM0 ****************************************************** */

void PWM0_Value(uint16 Duty_Cycle)
{
	#if Timer0_Mode    ==       TIMER_Fast_PWM_Mode
	{
		#if PWM0_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR0 = (Duty_Cycle * 256 / 100) - 1;
		}
		
		#elif PWM0_Mode    ==   PWM_Inverted_Mode
		{
			OCR0 = 255-(Duty_Cycle * 256 /100);
		}
		#endif
	}
	
	#elif Timer0_Mode    ==       TIMER_PWM_Phase_Correct_Mode
	{
		#if PWM0_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR0 = (Duty_Cycle * 256 / 100);
		}
		
		#elif PWM0_Mode    ==   PWM_Inverted_Mode
		{
			OCR0 = 255-(Duty_Cycle * 255 /100);
		}
		#endif
	}
	#endif
}

/* *************************************************************************** TIMER1 *********************************************************************** */

void timer1_init(void)
{
	#if  Timer1_Mode    ==      TIMER_Normal_Mode
	{
		CLR_BIT(TCCR1A,0);
		CLR_BIT(TCCR1A,1);
		CLR_BIT(TCCR1B,3);
		CLR_BIT(TCCR1B,4);
		#if interrupt1_state  ==    interrupt_Enable
		{
			SET_BIT(SREG ,7);
			SET_BIT(TIMSK,2);
		}
		#endif
	}
	
	#elif Timer1_Mode    ==      TIMER_CTC_Mode
	{
		CLR_BIT(TCCR1A,0);
		CLR_BIT(TCCR1A,1);
		SET_BIT(TCCR1B,3);
		CLR_BIT(TCCR1B,4);
		#if interrupt1_state  ==    interrupt_Enable
		{
			SET_BIT(SREG ,7);
			SET_BIT(TIMSK,4);
		}
		#endif
	}
	
	#elif Timer1_Mode    ==      TIMER_PWM_Phase_Correct_Mode
	{
		SET_BIT(TCCR1A,0);
		SET_BIT(TCCR1A,1);
		CLR_BIT(TCCR1B,3);
		SET_BIT(TCCR1B,4);
	}
	
	#elif Timer1_Mode    ==      TIMER_PWM_Phase_Correct_ICR_Mode
	{
		CLR_BIT(TCCR1A,0);
		SET_BIT(TCCR1A,1);
		CLR_BIT(TCCR1B,3);
		SET_BIT(TCCR1B,4);
	}
	
	#elif Timer1_Mode    ==      TIMER_Fast_PWM_Mode
	{
		SET_BIT(TCCR1A,0);
		SET_BIT(TCCR1A,1);
		SET_BIT(TCCR1B,3);
		SET_BIT(TCCR1B,4);
		#if   PWM1_Mode    ==   PWM_NON_Inverted_Mode
		{
			SET_BIT(TCCR1A,7);
			CLR_BIT(TCCR1A,6);
		}
		#elif PWM1_Mode    ==   PWM_Inverted_Mode
		{
			SET_BIT(TCCR1A,7);
			SET_BIT(TCCR1A,6);
		}
		#endif
	}
	
	#elif Timer1_Mode    ==      TIMER_Fast_PWM_ICR_Mode
	{
		CLR_BIT(TCCR1A,0);
		SET_BIT(TCCR1A,1);
		SET_BIT(TCCR1B,3);
		SET_BIT(TCCR1B,4);
		#if   PWM1_Mode    ==   PWM_NON_Inverted_Mode
		{
			SET_BIT(TCCR1A,7);
			CLR_BIT(TCCR1A,6);
		}
		#elif PWM1_Mode    ==   PWM_Inverted_Mode
		{
			SET_BIT(TCCR1A,7);
			SET_BIT(TCCR1A,6);
		}
		#endif
	}
	#endif
}

void timer1_start(void)
{
	 #if  Timer1_CLK     ==       Pres_CLK
	 {
		 SET_BIT(TCCR1B,0);
		 CLR_BIT(TCCR1B,1);
		 CLR_BIT(TCCR1B,2);
	 }
	 #elif  Timer1_CLK     ==       Pres_CLK_8
	 {
		 CLR_BIT(TCCR1B,0);
		 SET_BIT(TCCR1B,1);
		 CLR_BIT(TCCR1B,2);
	 }
	 #elif  Timer1_CLK     ==       Pres_CLK_64
	 {
		 SET_BIT(TCCR1B,0);
		 SET_BIT(TCCR1B,1);
		 CLR_BIT(TCCR1B,2);
	 }
	 #elif  Timer1_CLK     ==       Pres_CLK_256
	 {
		 CLR_BIT(TCCR1B,0);
		 CLR_BIT(TCCR1B,1);
		 SET_BIT(TCCR1B,2);
	 }
	 #elif  Timer1_CLK     ==      Pres_CLK_1024
	 {
		 SET_BIT(TCCR1B,0);
		 CLR_BIT(TCCR1B,1);
		 SET_BIT(TCCR1B,2);
	 }
	 #endif
	 
 }

 void timer1_stop(void)
 {
	 CLR_BIT(TCCR1B,0);
	 CLR_BIT(TCCR1B,1);
	 CLR_BIT(TCCR1B,2);
 }
 

void timer1_delay(uint32);

void timer1_Set_CallBack(void (*ptr)(void));

/* ********************************************* PWM1 ************************************************* */

void PWM1_Value(uint32 Duty_Cycle)
{
	
	#if Timer1_Mode    ==       TIMER_Fast_PWM_Mode
	{
		#if PWM1_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR1A  = (Duty_Cycle * 65536 / 100) - 1;
		}
		
		#elif PWM1_Mode    ==   PWM_Inverted_Mode
		{
			OCR1A  = 65535-(Duty_Cycle * 65536 /100);
		}
		#endif
	}
	
	#elif Timer1_Mode    ==       TIMER_PWM_Phase_Correct_Mode
	{
		#if PWM1_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR1A  = (Duty_Cycle * 35536 / 100);
		}
		
		#elif PWM1_Mode    ==   PWM_Inverted_Mode
		{
			OCR1A  = 65535-(Duty_Cycle * 65535 /100);
		}
		#endif
	}
	#endif
	
}

void PWM1_ICR_Value(float32 Duty_Cycle,uint16 Top_Value)
{
	ICR1=Top_Value;
	
	#if Timer1_Mode    ==      TIMER_Fast_PWM_ICR_Mode
	{
		#if PWM1_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR1A  = (Duty_Cycle * (Top_Value+1) / 100) - 1;
		}
		
		#elif PWM1_Mode    ==   PWM_Inverted_Mode
		{
			OCR1A  = Top_Value-(Duty_Cycle * (Top_Value+1) /100);
		}
		#endif
	}
	
	#elif Timer1_Mode    ==       TIMER_PWM_Phase_Correct_ICR_Mode
	{
		#if PWM1_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR1A  = (Duty_Cycle * (Top_Value+1) / 100);
		}
		
		#elif PWM1_Mode    ==   PWM_Inverted_Mode
		{
			OCR1A  = Top_Value-(Duty_Cycle * Top_Value /100);
		}
		#endif
	}
	#endif
	
}

void PWM1_Set_CallBack(void (*ptr)(void));

/* ************************************************ ICU *************************************************** */

void ICU_Init (void)
{
	CLR_BIT(DDRD,6);
	ICR1 = 0x0000;
	SET_BIT(TCCR1B,6);
	SET_BIT(TIMSK,5);
	SET_BIT(SREG,7);
	
}

/* ***************************************************************************** TIMER2 ********************************************************************* */

void timer2_init(void)
{
	CLR_BIT(TCCR2,7);
	
	#if     Timer2_Mode    ==       TIMER_Normal_Mode
	{
		CLR_BIT(TCCR2,6);
		CLR_BIT(TCCR2,3);
		
		#if interrupt2_state  ==    interrupt_Enable
		{
			SET_BIT(SREG ,7);
			SET_BIT(TIMSK,6);
		}
		#endif
	}
	#elif   Timer2_Mode    ==       TIMER_CTC_Mode
	{
		CLR_BIT(TCCR2,6);
		SET_BIT(TCCR2,3);
		
		#if interrupt2_state  ==    interrupt_Enable
		{
			SET_BIT(SREG ,7);
			SET_BIT(TIMSK,7);
		}
		#endif
	}
	
	#elif   Timer2_Mode    ==       TIMER_Fast_PWM_Mode
	{
		SET_BIT(TCCR2,6);
		SET_BIT(TCCR2,3);
		
		#if   PWM2_Mode    ==   PWM_NON_Inverted_Mode
		{
			SET_BIT(TCCR2,5);
			CLR_BIT(TCCR2,4);
		}
		#elif PWM2_Mode    ==   PWM_Inverted_Mode
		{
			SET_BIT(TCCR2,5);
			SET_BIT(TCCR2,4);
		}
		#endif
	}
	#elif   Timer2_Mode    ==       TIMER_PWM_Phase_Correct_Mode
	{
		SET_BIT(TCCR2,6);
		CLR_BIT(TCCR2,3);
		
		#if   PWM2_Mode    ==   PWM_NON_Inverted_Mode
		{
			SET_BIT(TCCR2,5);
			CLR_BIT(TCCR2,4);
		}
		#elif PWM2_Mode    ==   PWM_Inverted_Mode
		{
			SET_BIT(TCCR2,5);
			SET_BIT(TCCR2,4);
		}
		#endif
	}
	#endif
	
}



void timer2_start(void)
{
	
	#if  Timer2_CLK     ==       Pres_CLK
	{
		SET_BIT(TCCR2,0);
		CLR_BIT(TCCR2,1);
		CLR_BIT(TCCR2,2);
	}
	#elif  Timer2_CLK     ==       Pres_CLK_8
	{
		CLR_BIT(TCCR2,0);
		SET_BIT(TCCR2,1);
		CLR_BIT(TCCR2,2);
	}
	#elif  Timer2_CLK     ==       Pres_CLK_64
	{
		SET_BIT(TCCR2,0);
		SET_BIT(TCCR2,1);
		CLR_BIT(TCCR2,2);
	}
	#elif  Timer2_CLK     ==       Pres_CLK_256
	{
		CLR_BIT(TCCR2,0);
		CLR_BIT(TCCR2,1);
		SET_BIT(TCCR2,2);
	}
	#elif  Timer2_CLK     ==      Pres_CLK_1024
	{
		SET_BIT(TCCR2,0);
		CLR_BIT(TCCR2,1);
		SET_BIT(TCCR2,2);
	}
	#endif
	
}

void timer2_stop(void)
{
	CLR_BIT(TCCR2,0);
	CLR_BIT(TCCR2,1);
	CLR_BIT(TCCR2,2);
	
}


void timer2_delay(uint32 delay_ms )
{
	#if Timer2_Mode     ==      TIMER_Normal_Mode
	{
		uint8 tick_time = Timer2_CLK /Freq;    // micro second
		
		uint32 num_tick = (delay_ms*1000)/tick_time;
		
		Timer2_over_flow = num_tick/256;
		
		init_value = 256 -(   (  (float32) num_tick/256 - Timer2_over_flow  ) * 256  );
		
		TCNT2 = init_value;
		
		Timer2_over_flow++;
	}
	
	#elif Timer2_Mode     ==      TIMER_CTC_Mode
	{
		uint8 tick_time = Timer2_CLK /Freq;    // micro second
		
		uint32 num_tick = (delay_ms*1000)/tick_time;
		
		Timer2_Compare_Match = num_tick/255;
		
		init_value =    ((  (float32) num_tick/255 - Timer2_Compare_Match  ) * 255)-1;
		
		OCR2 = init_value;
		
		Timer2_Compare_Match++;
	}
	#endif
}

void timer2_Set_CallBack(void (*ptr)(void))
{
	Timer2_Ptr = ptr;
}

/* ************************************************ PWM2 ****************************************************** */

void PWM2_Value(uint16 Duty_Cycle)
{
	#if Timer2_Mode    ==       TIMER_Fast_PWM_Mode
	{
		#if PWM2_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR2 = (Duty_Cycle * 256 / 100) - 1;
		}
		
		#elif PWM2_Mode    ==   PWM_Inverted_Mode
		{
			OCR2 = 255-(Duty_Cycle * 256 /100);
		}
		#endif
	}
	
	#elif Timer2_Mode    ==       TIMER_PWM_Phase_Correct_Mode
	{
		#if PWM2_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR2 = (Duty_Cycle * 256 / 100);
		}
		
		#elif PWM2_Mode    ==   PWM_Inverted_Mode
		{
			OCR2 = 255-(Duty_Cycle * 255 /100);
		}
		#endif
	}
	#endif
}

/* *************************************************************************** ISR0 ************************************************************************ */

ISR (TIMER0_OVF_vect)
{
	count0++;
	while (count0 == Timer0_over_flow)
	{
		(*Timer0_Ptr)();
		TCNT0 = init_value;
		count0=0;
	}
}

ISR(TIMER0_COMP_vect)
{
	count0++;
	while (count0 == Timer0_Compare_Match)
	{
		(*Timer0_Ptr)();
		OCR0 = init_value;
		count0=0;
	}
	
}

/* ************************************************************************** ISR1 ********************************************************************** */

ISR(TIMER1_CAPT_vect)
{

	++ICU_Counter;
	if(ICU_Counter==1)
	{
		Timer1_over_flow=0;
		C1=ICR1;
		CLR_BIT(TCCR1B,6);
		
	}
	else if(ICU_Counter==2)
	{
		C2=ICR1+(Timer1_over_flow * 65535);
		SET_BIT(TCCR1B,6);
	}
	else if(ICU_Counter==3)
	{
		
		C3=ICR1+(Timer1_over_flow * 65535);
		CLR_BIT(TIMSK,5);
		CLR_BIT(TIMSK,2);
	}
}

ISR(TIMER1_OVF_vect)
{
	
	Timer1_over_flow++;
}