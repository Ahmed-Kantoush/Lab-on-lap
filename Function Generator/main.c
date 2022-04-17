/*
 * main.c
 *
 *  Created on: 22 Apr 2020
 *      Author: Ahmed
 */

//Name: Ahmed Kantoush

#include "avr\io.h"
#include "avr\interrupt.h"
#include <math.h>

#define freq 5.0 //setting frequency in hertz
#define amp 5 //setting amplitude in volts
char type = 1;
//Wave form type
//1: Sine wave
//2: Rect wave
//3: Tri wave

float time = 0.0;
float half = 1/(freq*2);
float period = 1/freq;
unsigned char i = 0;

int main()
{
	DDRB |= 1 << 3; //Set Pwm pin as output
	TCCR0 |= (1 << COM01) | (1 << WGM01) | (1 << WGM00) | (1 << CS01) | 0;
	//Initialising timer 2A as fast pwm with no pre-scaler
	TIMSK |= 1 << OCIE0; //Setting timer 2 interrupt on compare match
	MCUCR = (1 << ISC01); //Interrupt on falling edge;
	GICR = 1 << INT0; //Enabling interrupt
	sei(); //Enabling global interrupt
	while(1)
	{
		if (time >= period) // Reset time to zero if 1/2 cycle is complete
			time = 0;

		if (type == 1) //Sine wave
			i = amp*sin(2*M_PI*freq/2*time)*255/5; //Update Pwm with next value
		else if (type == 2)
			if (time < half)
				i = amp*255/5;
			else
				i = 0;
	    else
			if (time < half)
				i = time*(amp/half)*255/5;
			else
				i = (amp - (time-half)*(amp/half))*255/5;
	}

	return 0;
}

ISR(INT0_vect) //ISR for interrupt pin 0 to change waveform type
{

  if (type == 3)
    type = 1;
  else
    type++;
  time = 0; //Resets time
}


ISR(TIMER0_COMP_vect) //ISR for timer interrupt to update pwm
{
  time += 0.000256; // Add time passed when timer overflows
  OCR0 = i;
}
