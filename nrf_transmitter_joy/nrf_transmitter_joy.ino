// Date: 10-mar-2018
// Author: Isidoro Gayo
// Name: Wireless robot control
// Fillenme: nrf_transmitter_bot.ino

// --------------------------------------------------------------------
// Description: Basic control for a wireless robot.
// A couple of nRF24 modules send and recieve control commands
// from a joystick. Used a L298 motor driver.
// 
// WARNING!! Both nRF modules must be powered with 3.3v.
//
// This is the sketch to be loaded in the arduino controlled joystick.
// --------------------------------------------------------------------

// Published under the terms of GNU/GPL v3.0 or higher license

// Library used: TMRh20/RF24, https://github.com/tmrh20/RF24/
// nRF24 theory and examples: Dejan Nedelkovski, www.HowToMechatronics.com


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(8, 9);         // CE, CSN. MOSI, MISO and CLK signals are, 
                          // by default, in 11,12 and 13 pins.
const byte address[6] = "00001";  // Address for transmitter module. Must be
                                  // paired with the reciever one.
char xyData[32] = "";             // Commands sent go here
String xAxis, yAxis;              // Movement commands (raw reading from pot)



// -------------------------- Code Entry --------------------

void setup(){
  
  Serial.begin(9600);
  
  radio.begin();                  // Init transmitter module
  radio.openWritingPipe(address); // Set the channel and reciever address.
  radio.setPALevel(RF24_PA_MIN);    
  radio.stopListening();
}
void loop(){

  xAxis = analogRead(A0); // Read Joysticks X-axis
  yAxis = analogRead(A1); // Read Joysticks Y-axis
  // X value
  xAxis.toCharArray(xyData, 5);           // Convert the raw x value into a  
                                          // character array ready to be sent.
  radio.write(&xyData, sizeof(xyData));   // Send the array data nRF24 
                                          // reciever module...
  yAxis.toCharArray(xyData, 5);           // ... and repeat for y value.
  radio.write(&xyData, sizeof(xyData));
  
  delay(20);              // Wait a bit (Needed by nRF24 module) 
}

// ------------------------- End of Code (EoC) ----------------------

