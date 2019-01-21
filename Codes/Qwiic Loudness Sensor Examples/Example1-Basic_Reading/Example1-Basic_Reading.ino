/******************************************************************************
  Zio Qwiic Loudness Sensor Example 1 - Basic Reading
  Harry
  September 25, 2018

  Default address is 0x38.

******************************************************************************/
#include <Wire.h>

#define COMMAND_LED_OFF     0x00
#define COMMAND_LED_ON      0x01
#define COMMAND_GET_VALUE        0x05
#define COMMAND_NOTHING_NEW   0x99

const byte qwiicAddress = 0x38;     //Default Address
uint16_t ADC_VALUE=0;
void setup() {
  Serial.begin(9600);
  Serial.println("Zio Qwiic Loudness Sensor Master Awake");

  Wire.begin();
  testForConnectivity();
  ledOn();
  delay(1000);
}

void loop() {
  get_value();
  ledOn();
  delay(100);
  ledOff();
  delay(100);
}

// LED is off, and a -1 if an error occurred.
void get_value() {
  Wire.beginTransmission(qwiicAddress);
  Wire.write(COMMAND_GET_VALUE); // command for status
  Wire.endTransmission();    // stop transmitting //this looks like it was essential.

  Wire.requestFrom(qwiicAddress, 2);    // request 1 bytes from slave device qwiicAddress

  while (Wire.available()) { // slave may send less than requested
  uint8_t ADC_VALUE_L = Wire.read(); 
//  Serial.print("ADC_VALUE_L:  ");
//  Serial.println(ADC_VALUE_L,DEC);
  uint8_t ADC_VALUE_H = Wire.read();
//  Serial.print("ADC_VALUE_H:  ");
//  Serial.println(ADC_VALUE_H,DEC);
  ADC_VALUE=ADC_VALUE_H;
  ADC_VALUE<<=8;
  ADC_VALUE|=ADC_VALUE_L;
  Serial.print("ADC_VALUE:  ");
  Serial.println(ADC_VALUE,DEC);
  }
  uint16_t x=Wire.read(); 
}

void ledOn() {
  Wire.beginTransmission(qwiicAddress);
  Wire.write(COMMAND_LED_ON);
  Wire.endTransmission();
}

void ledOff() {
  Wire.beginTransmission(qwiicAddress);
  Wire.write(COMMAND_LED_OFF);
  Wire.endTransmission();
}


// testForConnectivity() checks for an ACK from an Sensor. If no ACK
// program freezes and notifies user.
void testForConnectivity() {
  Wire.beginTransmission(qwiicAddress);
  //check here for an ACK from the slave, if no ACK don't allow change?
  if (Wire.endTransmission() != 0) {
    Serial.println("Check connections. No slave attached.");
    while (1);
  }
}
