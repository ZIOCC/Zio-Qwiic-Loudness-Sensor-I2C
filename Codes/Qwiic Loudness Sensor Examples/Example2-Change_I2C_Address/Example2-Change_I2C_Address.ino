/******************************************************************************
  Zio Qwiic Loudness Sensor Example 2 - Change I2C Address
  Harry
  September 25, 2018

  This sketch demonstrates how to change the I2C Address
  of the Zio Qwiic Loudness Sensor. The new address will be saved to the
  Sensor's EEPROM.

  The factory default slave address is 0x38.

  Note: If the Zio Qwiic Loudness Sensor is connected but unresponsive, 
  try Example 3 - I2C_Scanner to find the current address of the Sensor.

  Only valid address can be saved to the Sensor's EEPROM. If an invalid address is
  sent, the Sensor will ignore the address change without notifying the user.

******************************************************************************/
#include <Wire.h>
#define COMMAND_LED_OFF 				  0x00
#define COMMAND_LED_ON 				    0x01
#define COMMAND_CHANGE_ADDRESS 			0x03

volatile byte qwiicAddress = 0x38;     //Default Address

void setup() {
  Serial.begin(9600);
  Serial.println("Zio Qwiic Loudness Sensor Master Awake");
  Wire.begin(); // join the I2C Bus

  Wire.beginTransmission(qwiicAddress); // transmit to device
  //check here for an ACK from the slave
  if (Wire.endTransmission() != 0 ) {
    Serial.println("Check Connections. Slave not found.");
  }

  Serial.println("Zio Qwiic Loudness Sensor found!");

  ledOn(); // Turn on the LED (at address 0x28)
  delay(1000);
  byte error = changeAddress(0x29); // Change the Sensor's address to 0x29
  ledOff(); // Turn off the LED (at address 0x29)
  delay(1000);
  changeAddress(0x00); // Try to change address to Invalid Address.

  if (error != true) {
    Serial.println("!!!!! invalid address" );
  }
  Serial.println("success");
}

void loop() {
}
// changeAddress() takes a 7 bit I2C Address
// and writes it to the Sensor. This function
// checks to see if the address is between
// 0x07 and 0x78. If valid, the new address is
// saved to the Sensor's EEPROM. If not valid
// address is not changed and is ignored.
// This function returns true if successful and
// false if unsuccessful.
boolean changeAddress(byte address) {
  Wire.beginTransmission(qwiicAddress);
  //check here for an ACK from the slave
  if (Wire.endTransmission()  != 0) {
    Serial.println("Check Connections. No slave found.");
    return (false);
  }

  //check if valid address.
  if (address < 0x07 || address > 0x78) {
    Serial.println("Invalid I2C address");
    return (false);
  }
  //valid address
  Wire.beginTransmission(qwiicAddress);
  Wire.write(COMMAND_CHANGE_ADDRESS);
  qwiicAddress = address;
  Wire.write(qwiicAddress);
  Wire.endTransmission();
  return (true); //Success!
}

// ledOn() turns on the relay at the SLAVE_ADDRESS
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void ledOn() {
  Wire.beginTransmission(qwiicAddress);
  Wire.write(COMMAND_LED_ON);
  Wire.endTransmission();
}

// ledOff() turns off the relay at the SLAVE_ADDRESS
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void ledOff() {
  Wire.beginTransmission(qwiicAddress);
  Wire.write(COMMAND_LED_OFF);
  Wire.endTransmission();
}

