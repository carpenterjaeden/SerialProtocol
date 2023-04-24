#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"

void initPWMTimer3(){ //sets timer 3 to 10 bit PWM (non inverting)

    //Set header pin 3 (OC3C) to output
    DDRE |= (1 << DDE5);
    
    //set timer 3 into fast pwm mode 10-bit
    TCCR3B |= ((1 << WGM33)|(1 << WGM32));
    TCCR3A |= ((1 << WGM31)|(1 << WGM30));

    //turn on fast pwm non-inverting mode COM3C1:0 = 10 (C because C is our output)
    TCCR3A &= ~(1 << COM3C0);
    TCCR3A |= (1 << COM3C1); 
}


//change the frequency of the pwm by changing the top (OCR3A)
void changeFrequency(unsigned int freq){

    OCR3AH = ((16000000 / freq) >> 8); //set the high byte of OCR3A to determine the top value (period)
    OCR3AL = (16000000 / freq); //set the low byte of OCR3A to determine the top value (period)
    //OCR3C should be the top value divided by 2, to maintain 50% duty cycle
    OCR3CH = OCR3AH >> 1; //right shifting 1 divides by 2
    OCR3CL = OCR3AL >> 1; //right shifting 1 divides by 2
}

void turnOnAlarm(){ //turn on the alarm
    TCCR3B |= ( 1 << CS30); //turn on pwm with prescalar 1
}

void turnOffAlarm(){
    OCR3AH = (0); //set the high byte of OCR3A to determine the top value (period)
    OCR3AL = (0);
}