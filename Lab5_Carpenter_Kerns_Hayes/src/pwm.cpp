#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"

void initPWMTimer3(){

//initialize pin
    DDRE |= (1 << DDE5);
//Set timer counter control register to fast PWM 10-bit mode
    TCCR3A |= (1 << WGM30);
    TCCR3A |= (1 << WGM31);
    TCCR3B |= (1 << WGM32);
    TCCR3B &= ~(1 << WGM33);
// set timer counter control register to prescaler of none
    TCCR3B |= (1 << CS30); 
    TCCR3B &= ~(1 << CS31);
    TCCR3B &= ~(1 << CS32);
// set compare output mode non-inverting
    TCCR3A &= ~(1 << COM3A0);
    TCCR3A |= (1 << COM3A1);

    OCR3AH = 0;
    OCR3AL = 0;
    OCR3CH = 0;
    OCR3CL = 0;



    
}

void changeDutyCycle(unsigned int combo){

    OCR3AH = ((16000000/combo) >> 8);
    OCR3AL = (16000000/combo);
    OCR3CH = OCR3AH >> 1; //right shifting 1 divides by 2
    OCR3CL = OCR3AL >> 1; //right shifting 1 divides by 2
}
//turn off alarm by changing dd
void turnOffAlarm(){
    DDRE &= ~(1 << DDE5);
}
//turn on alarm by changing ddr
void turnOnAlarm(){
    DDRE |= (1 << DDE5);
}