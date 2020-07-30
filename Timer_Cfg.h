/*
 * Timer_Cfg.h
 *
 * Created: 7/17/2020 7:11:51 PM
 *  Author: 20112
 */ 


#ifndef TIMER_CFG_H_
#define TIMER_CFG_H_

#include "Timer.h"

#define Freq                16    // MEGA HERTEZ
 
#define Timer0_CLK            Pres_CLK_256
#define Timer1_CLK            Pres_CLK
#define Timer2_CLK            Pres_CLK_256

#define Timer0_Mode           TIMER_PWM_Phase_Correct_Mode 
#define Timer1_Mode           TIMER_Normal_Mode
#define Timer2_Mode           TIMER_Fast_PWM_Mode     

#define PWM0_Mode             PWM_NON_Inverted_Mode
#define PWM1_Mode             PWM_NON_Inverted_Mode
#define PWM2_Mode             PWM_NON_Inverted_Mode

#define interrupt0_state      interrupt_Disable 
#define interrupt1_state      interrupt_Enable 
#define interrupt2_state      interrupt_Disable 

#endif /* TIMER_CFG_H_ */