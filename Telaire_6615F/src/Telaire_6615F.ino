/*
 * Project Serial_Read_Write
 * Description:
 * Author:
 * Date:
 */
//

#include "Serial5/Serial5.h"
SYSTEM_MODE(MANUAL);

// packet to read CO2 (see app note)
char readCO2[] = {0xFF, 0XFE, 0X02, 0X02, 0X03}; //T66 read CO2 command: 5 bytes

//create an array to store the response
char response[] = {0, 0, 0, 0, 0};

void setup() {
	Cellular.off();

  // To print to screen over USB-Serial
	Serial.begin(9600);

	// Serial1 RX is connected to Arduino TX (1)
	// Serial1 TX is connected to Arduino RX (0)
  // Electron GND is connected to K-30 GND
	Serial5.begin(19200);

	delay(5000);

	Serial.println("Code started...");
  Serial.println("Demo of AN-126 Software Serial and K-30 Sensor");
}

void loop()
{
  //read CO2 every 10 seconds
  unsigned long co2 = read_Co2();
  Serial.print("CO2 ppm=");
  Serial.println(co2);
  delay(10000);
}

int read_Co2()
{
 byte buf[56];
 byte i=0;

 Serial5.write(0xFF);
 Serial5.write(0xFE);
 Serial5.write(0x02);
 Serial5.write(0x02);
 Serial5.write(0x03);
 delay(20);

 // put he two bit answer from the sensor into a buffer
 while(Serial5.available())
 {
 buf[i]=Serial5.read();
 // Serial.write(buf[i]);
 i++;
 }

 // do a bitwise operation to acomodate the two bits
 // and make them an integer
 int high = buf[3]; //high byte for value is 4th byte in packet in the packet
 int low = buf[4]; //low byte for value is 5th byte in the packet
 unsigned long co2Value = high*256+low; //Combine high byte and low byte with this formula to get value
 return co2Value;
}
