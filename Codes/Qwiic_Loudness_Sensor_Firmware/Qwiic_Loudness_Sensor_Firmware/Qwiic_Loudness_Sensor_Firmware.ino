/******************************************************************************
  Zio Qwiic Loudness Sensor Firmware
  Harry
  September 25, 2018

  This is the firmware on the Zio Qwiic Loudness Sensor. The default I2C Address is
  0x38;

******************************************************************************/
#include <TinyWire.h> //https://github.com/lucullusTheOnly/TinyWire
#include <EEPROM.h>

#define LED_PIN   3
#define ADC_PIN   A2
#define SETTING_LOCATION_ADDRESS	1

volatile byte qwiicAddress  =    0x38; //default

#define COMMAND_LED_OFF 			0x00
#define COMMAND_LED_ON 			0x01
#define COMMAND_CHANGE_ADDRESS 		0x03
#define COMMAND_GET_VALUE 				0x05
#define COMMAND_HAS_BEEN_CHECKED 	0x99

volatile byte command = COMMAND_HAS_BEEN_CHECKED;
volatile byte address = COMMAND_HAS_BEEN_CHECKED;
uint16_t ADC_VALUE=0;
uint8_t  ADC_VALUE_H=0,ADC_VALUE_L=0;

void setup() {
  //Read EEPROM, is it empty (0xFF)? or does it have a value?
  qwiicAddress =  EEPROM.read(SETTING_LOCATION_ADDRESS);
  if (qwiicAddress == 0xFF) {
    //EEPROM has never been written before, use the default address 0x38.
    qwiicAddress = 0x38; //default
  }

  TinyWire.begin(qwiicAddress);
  TinyWire.onReceive(receiveEvent); // register event
  TinyWire.onRequest(onI2CRequest);
}

void loop() {
}

/*========================================================*/
//        ISR
/*========================================================*/

// When the master initiates a command and data to slave
//		ie) the master says 0x01, then sends a 1, means command: 0x01 then the slave listens for the next thing, which is the LED state 1
void receiveEvent(int bytesReceived) {
  byte count = 0;
  while (TinyWire.available() > 0) {
    if (count == 0) {
      command = TinyWire.read();

      if (command == COMMAND_LED_ON) {
        digitalWrite(LED_PIN, HIGH);
        command = COMMAND_HAS_BEEN_CHECKED;
      }
      else if (command == COMMAND_LED_OFF) {
        digitalWrite(LED_PIN, LOW);
        command = COMMAND_HAS_BEEN_CHECKED;
      }
    }
    else if (count == 1) {
      if (command == COMMAND_CHANGE_ADDRESS) {
        byte address = TinyWire.read();

        if (address > 0x07 && address < 0x78) {
          //valid address, update and save to EEPROM
          qwiicAddress = address;

          EEPROM.write(SETTING_LOCATION_ADDRESS, qwiicAddress);
          TinyWire.begin(qwiicAddress);
        }
      }
    }
    else {
      TinyWire.read(); //read the data coming in but ignore it.
    }
    count++;
  }
}// end of receive ISR

//When the master requests data from the slave, this
//    ISR is triggered.

void onI2CRequest() {
   if (command == COMMAND_GET_VALUE) {
    ADC_VALUE=analogRead(ADC_PIN);
    ADC_VALUE_L=ADC_VALUE;
    ADC_VALUE_H=ADC_VALUE>>8; 
    TinyWire.send(ADC_VALUE_L);  
    TinyWire.send(ADC_VALUE_H);
    command = COMMAND_HAS_BEEN_CHECKED;
  }
}

