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
#include "I2C.h"
#include "spi.h"

// defines


/*
 * Define a set of states that can be used in the state machine using an enum.
 */
typedef enum {smile, frown} states;
states matrix = smile;

  unsigned int muteflag = 0;

  typedef enum {wait_press, debounce_press, wait_release, debounce_release} debounce;
//define global variable for debounce states
volatile debounce dbState = wait_press;

int main(){

  
  initTimer1();
  initPWMTimer3();
  initSwitchPD0();
  InitI2C();
  SPI_MASTER_Init();
  
	
  sei(); // Enable global interrupts.
  
  unsigned int result = 0;
  float voltage = 0;
	while (1) {

   //if reaches threshhold, trigger frown


    switch (matrix){
      case smile:
        //make the matrix smile
        //mute flag low
      break;
      case frown:
      //make the matrix frown
      //make the alarm chirp
      break;
      default:
      matrix = smile;
      break;
    }

  //Switch case for debounce states
  switch(dbState){
  //do nothing while waiting
    case wait_press:
    break;

  //debounce press adds delay and goes to wait_release
    case debounce_press:
    delayMs(1);
    dbState = wait_release;
    break;

  //Do nothing while waiting
    case wait_release:
    break;
  //After release, delay and then go back to waiting for press
    case debounce_release:
    delayMs(1);
    dbState = wait_press;
    //if frown, mute flag high
    break;

  }

	}
  return 0;
}


//Pin change interrupt: INT0 uses PORTD0
ISR(INT0_vect){

//if INT0 is triggered for press
if (dbState == wait_press){
  dbState = debounce_press;
}
//if INT0 is triggered for release
else if (dbState == wait_release){
  //change motor state to counting
  dbState = debounce_release;
}



}