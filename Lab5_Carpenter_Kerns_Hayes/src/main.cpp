// Author: Jaeden Carpenter
// Net ID: carpenterjaeden
// Date: 3/20/23
// Assignment: Lab 3
//----------------------------------------------------------------------//

// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//




#include <Arduino.h>
#include <avr/io.h>
#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "sevensegment.h"

// defines


/*
 * Define a set of states that can be used in the state machine using an enum.
 */
typedef enum {counting, motor} states;
states interrupt = motor;

  unsigned int portdhistory = 0;
int main(){

  initTimer0();
  initTimer1();
  initPWMTimer3();
  initSwitchPD0();
  initSevenSegment();



  
  sei(); // Enable global interrupts.
  
	while (1) {

//switch case to determine delay based on the state we are in (motor or longDelat)
    switch (interrupt){
      case motor:
        
        changeDutyCycle(ADCL + ((unsigned int)ADCH << 8));
      break;
      case counting:
        
        for (unsigned int i = 9; i >= 0; i--){
          displayNum(i);
          delayMs1(1000);
        }
        interrupt = motor;
      break;
      default:
      interrupt = motor;
      break;
    }

   

	}
  return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
//INT0 => PORTD0
ISR(INT0_vect){
unsigned int changedbits;
changedbits = PIND ^ portdhistory;
portdhistory = PIND;
if(changedbits & (1 << PD0))
{ interrupt = counting; }

}