/******************************************************************************
  Zio Qwiic Loudness Sensor Example 1 - Basic Reading
  Harry
  September 25, 2018

  Default address is 0x38.

******************************************************************************/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define COMMAND_LED_OFF     0x00
#define COMMAND_LED_ON      0x01
#define COMMAND_GET_VALUE        0x05
#define COMMAND_NOTHING_NEW   0x99

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const byte qwiicAddress = 0x38;     //Default Address
uint16_t ADC_VALUE=0;
void setup() {
  Serial.begin(9600);
  Serial.println("Zio Qwiic Loudness Sensor Master Awake");

  Wire.begin();
  testForConnectivity();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
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

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println("Loudness ADC Value:");
  display.print(ADC_VALUE);
  display.display();
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
