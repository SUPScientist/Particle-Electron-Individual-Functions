/*
 * Project Serial_Read_Write
 * Description:
 * Author:
 * Date:
 */
//

#include "Serial4/Serial4.h"

SYSTEM_MODE(MANUAL);

// packet to read CO2 (see app note)
char command[] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25};

//create an array to store the response
char response[] = {0, 0, 0, 0, 0, 0, 0};

void setup() {
	Cellular.off();

  // To print to screen over USB-Serial
	Serial.begin(9600);

	// Serial1 RX is connected to Arduino TX (1)
	// Serial1 TX is connected to Arduino RX (0)
  // Electron GND is connected to K-30 GND
	Serial4.begin(9600);

	delay(3000);
	Serial.println("Testing 1, 2...");
  Serial.println("Demo of AN-126 Software Serial and K-30 Sensor");
}

void loop() {
  //keep sending request until we start to get a response
  for (int i=0; i < 7; i++) {
    Serial4.write(command[i]);
    // Serial.println(command[i]);
  }

  delay(20); // ms

  // Read response back one byte at a time
  for (int i=0; i < 7; i++) {
    response[i] = Serial4.read();
    Serial.println(response[i]);
  }

  unsigned long valCO2 = getValue(response);

  Serial.print("Room CO2 = ");
  Serial.println(valCO2);
  // unsigned long valCO2 = getValue(response);
  // Serial.print("Co2 ppm = ");
  // Serial.println(valCO2);
  delay(2000);
}

unsigned long getValue(char packet[]) {
  int high = packet[3]; //high byte for value is 4th byte in packet in the packet
  int low = packet[4]; //low byte for value is 5th byte in the packet
  unsigned long val = high*256 + low; //Combine high byte and low byte with this formula to get value
  return val;
}
