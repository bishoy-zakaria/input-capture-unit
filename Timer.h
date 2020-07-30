/*
 * Timer.h
 *
 * Created: 7/17/2020 7:12:03 PM
 *  Author: 20112
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "ATMEGA32_Regs.h"
#include "BIT_Math.h"
#include "DIO.h"
#include "STD.h"

#define Pres_CLK                   1
#define Pres_CLK_8                 8
#define Pres_CLK_64                64
#define Pres_CLK_256               256
#define Pres_CLK_1024              1024

/* ********************************* TIMER_Modes **************************** */

#define TIMER_Normal_Mode                            0
#define TIMER_CTC_Mode                               1
#define TIMER_PWM_Phase_Correct_Mode                 2
#define TIMER_Fast_PWM_Mode                          3
#define TIMER_PWM_Phase_Correct_ICR_Mode             4
#define TIMER_Fast_PWM_ICR_Mode                      5

/* ********************************** TIMER0 ******************************* */

void timer0_init(void);

void timer0_start(void);

void timer0_stop(void);

void timer0_delay(uint32);

void timer0_Set_CallBack(void (*ptr)(void));

/* ************************************ TIMER1 ***************************** */

void timer1_init(void);

void timer1_start(void);

void timer1_stop(void);

void timer1_delay(uint32);

void timer1_Set_CallBack(void (*ptr)(void));

/* ********************************* TIMER2 ******************************* */

void timer2_init(void);

void timer2_start(void);

void timer2_stop(void);

void timer2_delay(uint32);

void timer2_Set_CallBack(void (*ptr)(void));

/* ******************************** PWM_Direction ********************* */

#define PWM_NON_Inverted_Mode      0
#define PWM_Inverted_Mode          1

/* ******************************* INTERRRUPT ************************ */

#define interrupt_Disable          0
#define interrupt_Enable           1

/* ****************************** PWM0 *********************************** */

void PWM0_Value(uint16 );
void PWM0_Set_CallBack(void (*ptr)(void));

/* ******************************* PWM1 ********************************** */

void PWM1_Value(uint32 );
void PWM1_ICR_Value(float32 Duty_Cycle,uint16 Top_Value);
void PWM1_Set_CallBack(void (*ptr)(void));

/* ****************************** PWM2 *********************************** */

void PWM2_Value(uint16 );
void PWM2_Set_CallBack(void (*ptr)(void));

/* ******************************** ICU ********************************* */

void ICU_Init (void);


#endif /* TIMER_H_ */