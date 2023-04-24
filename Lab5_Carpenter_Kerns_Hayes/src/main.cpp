// Author: Jaeden Carpenter, Sam Kerns, Malcolm Hayes
// Net ID: carpenterjaeden, s4k3, malcolmhayes
// Date: 5/1/23
// Assignment: Lab 5
//----------------------------------------------------------------------//

// Description: In this lab, you will work in a ** team ** of 3-4 people.
//
// You will use the MPU 6050 accelerometer module and I2C serial
// communication protocol to read and print the measured X, Y, and Z data
// accelerometer from the module. You will also detect changes in either Y
// and Z axis rotation above a predetermined threshold level (~ 45 degrees
// for each axis). This threshold level will determine the state of the
// 8x8 LED matrix display and whether a buzzer alarm sounds off.
// You will interface with the 8 x 8 LED matrix using the SPI
// communication protocol. While the MPU 6050 accelerometer is below the
// threshold level that you determine, you will display a smiley face on
// the 8 x 8 LED matrix. When the MPU 6050 accelerometer goes above the
// threshold level, the smiley face turns to a frowny face on the 8 x 8
// led matrix display.
//
// You will also implement a buzzer alarm that produces a chirping sound
// when the accelerometer sensor goes above a certain threshold value.
//
// A push button switch will silence the piezo alarm.
//
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

  boolean muteflag = 0;

  typedef enum {wait_press, debounce_press, wait_release, debounce_release} debounce;
//define global variable for debounce states
volatile debounce dbState = wait_press;

int main(){

  
  initTimer1();
  initPWMTimer3();
  initSwitchPD2();
  initI2C();
  SPI_MASTER_Init();
  Serial.begin(9600);
  
	
  sei(); // Enable global interrupts.
  
StartI2C_Trans(104);

//status = TWSR & 0xF8;

write(0x6B);// address on SLA for Power Management
write(0); // send data to Wake up from sleep mode


//status = TWSR & 0xF8;


StopI2C_Trans();

  //unsigned int result = 0;
  //float voltage = 0;
	while (1) {

    //Read X
    Read_from(104, 59);
    unsigned int x = Read_data();
    Read_from(104, 60);
    x = (x<<8) + Read_data();
    //Read Y
    Read_from(104, 61);
    unsigned int y = Read_data();
    Read_from(104, 62);
    y = (y<<8) + Read_data();
    //Read Z
    Read_from(104, 63);
    unsigned int z = Read_data();
    Read_from(104, 64);
    z = (z<<8) + Read_data();
    //Serial prints
    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
   //if reaches threshhold, trigger frown

    if ((y > 350) | (z > 350)){
      matrix = frown;
    }

    switch (matrix){
      case smile:
        displaySmile();
        muteflag = 0;
      break;
      case frown:
        displayFrown();
        //make the alarm chirp
        if (muteflag == 0){
        changeDutyCycle(350);
        }
        else{
          changeDutyCycle(0);
        }
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
ISR(INT2_vect){

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