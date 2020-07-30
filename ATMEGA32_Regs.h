


#ifndef ATMEGA32_REG_H_
#define ATMEGA32_REG_H_

#include "STD.h"

#define PORTA (*(volatile uint8*)(0x3B))
#define PINA  (*(volatile uint8*)(0x39))
#define DDRA  (*(volatile uint8*)(0x3A))

#define PORTB (*(volatile uint8*)(0x38))
#define PINB  (*(volatile uint8*)(0x36))
#define DDRB  (*(volatile uint8*)(0x37))

#define PORTC (*((volatile uint8*)(0x35)))
#define PINC  (*((volatile uint8*)(0x33)))
#define DDRC  (*((volatile uint8*)(0x34)))

#define PORTD (*(volatile uint8*)(0x32))
#define PIND  (*(volatile uint8*)(0x30))
#define DDRD  (*(volatile uint8*)(0x31))

/* ************************************  ADC  **************************************** */

#define ADMUX          (*(volatile uint8*)(0x27))
#define ADCSRA         (*(volatile uint8*)(0x26))
#define ADCH           (*(volatile uint8*)(0x25))
#define ADCL           (*(volatile uint8*)(0x24))
#define ADC_ADJUST     (*(volatile uint16*)(0x24))
#define SFIOR          (*(volatile uint8*)(0x50))

/* ************************************* GENERAL INTRRURT *************************** */

#define SREG         (*(volatile uint8*)(0x5F))

/* *************************************** TIMER0 ************************************ */

#define TCCR0        (*(volatile uint8*)(0x53))
#define TCNT0        (*(volatile uint8*)(0x52))
#define OCR0         (*(volatile uint8*)(0x5C))
#define TIMSK        (*(volatile uint8*)(0x59))
#define TIFR         (*(volatile uint8*)(0x58))

/* *************************************** TIMER1 ************************************ */

#define TCCR1A        (*(volatile uint8*)(0x4F))
#define TCCR1B        (*(volatile uint8*)(0x4E))
#define TCNTL1        (*(volatile uint16*)(0x4C))
#define OCR1A         (*(volatile uint16*)(0x4A))
#define ICR1          (*(volatile uint16*)(0x46))


/* *************************************** TIMER2 ************************************ */

#define TCCR2        (*(volatile uint8*)(0x45))
#define TCNT2        (*(volatile uint8*)(0x44))
#define OCR2         (*(volatile uint8*)(0x43))

#endif /* ATMEGA32_REG_H_ */