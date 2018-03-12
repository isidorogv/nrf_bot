// Date: 10-mar-2018
// Author: Isidoro Gayo
// Name: Wireless robot control
// Fillenme: nrf_reciever_bot.ino

// -------------------------------------------------------------
// Description: Basic control for a wireless robot.
// A couple of nRF24 modules send and recieve control commands
// from a joystick. Used a L298 motor driver.
// 
// WARNING!! Both nRF modules must be powered with 3.3v.
//
// This is the sketch to be loaded on controlled robot.
// -------------------------------------------------------------

// Published under the terms of GNU/GPL v3.0 or higher license

// Library used: TMRh20/RF24, https://github.com/tmrh20/RF24/
// nRF24 theory and examples: Dejan Nedelkovski, www.HowToMechatronics.com


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// motors control
#define len 2   // Enables left motor driver by means of a PMW signal.
#define in1 4   // Digital controlled output for the 1st half-bridge driver.
#define in2 5   // Digital controlled output for the 2nd half-bridge driver.
#define ren 3   // Same for right motor...
#define in3 6
#define in4 7

// nRf module management
RF24 radio(8,9);                  // CE, CSN. MOSI, MISO and CLK signals are, 
                                  // by default, in 11,12 and 13 pins.
const byte address[6] = "00001";  // Address for reciever module.
char receivedData[32] = "";       // Recieved commands go here.

// Speed and movement motor variables
int  xAxis, yAxis;        // Potentiometer readings, controls movements 
                          // on motors.
const int MAXSPEED=255;   // Max. speed allowed. It depends on your 
                          // motors/drivers. Set to your own!
const int offset=20;      // Value range where bot stays stopped 
                          // (joystick is around central position).
const int cpot=511;       // Analog reading when potentiometer is centered.


// -------------------------- Code Entry --------------------

void setup(){

  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // init motors controls
  pinMode(len, OUTPUT);
  pinMode(ren, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}
    
void loop(){

  int lspeed,rspeed=0;    // motors stopped by default.
  
  if (radio.available()){
    
    radio.read(&receivedData, sizeof(receivedData));  // If nRF24 module 
                                                      // received data, read
                                                      // these data and put them  
                                                      // into character array.
    xAxis = atoi(&receivedData[0]);   // Convert data from the character array 
                                      // (received X value) into integer
    delay(10);      // Wait a bit and ...
    
    // ...repeat for y axis
    radio.read(&receivedData, sizeof(receivedData));
    yAxis = atoi(&receivedData[0]);
    delay(10);
  }
  // The following code determines de bot movement
  // FORWARD, BACKWARD, STOP, TURN RIGHT or LEFT

  // ---- Lineal movements ----
  // BACKWARD
  if (yAxis<cpot-offset){
    // int yAxis must be mapped first from 
    // 0 for minimum speed(cpot-offset value) to max speed(0)
    // The robot must go backward at 'yAxis' speed

    // Set Motor A backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    
    lspeed=map(yAxis, cpot-offset, 0, 0, MAXSPEED);
    rspeed=lspeed;
  }
  // FORWARD
  else if (yAxis>cpot+offset){
    // int yAxis variable must be mapped first within
    // 0 speed(cpot+offset) to maximun speed (1023-1026)
    // bot must go forward at 'yAxis' speed

    // Both motors go forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    lspeed=map(yAxis,cpot+offset,1026,0,MAXSPEED);
    rspeed=lspeed;
  }
  // STOP
  else{
    // if the joystick is around central position, the bot 
    // will stay stopped, so stop motors inmediately whether they are on.
    lspeed=0;
    rspeed=0;
  }

  // ---- Turns ----
  // RIGHT
  if (xAxis<cpot-offset){
    // Left motor goes forward...
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // ...and right motor goes backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    lspeed=map(xAxis,cpot-offset,0,0,MAXSPEED);
    rspeed=lspeed;
  }
  // LEFT
  if (xAxis>cpot+offset){
    // Left motor goes backward...
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // ...and right motor goes forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    lspeed=map(xAxis,cpot+offset,1026,0,MAXSPEED);
    rspeed=lspeed;
  }
  // perform the robot movement
  analogWrite(len, lspeed);
  analogWrite(ren, rspeed);
}

// ------------------------- End of Code (EoC) ----------------------

