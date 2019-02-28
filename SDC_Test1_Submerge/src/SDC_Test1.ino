/*
 * Project SDC_Test0
 * Description:
 * Author:
 * Date:
 */

 #include "SdFat.h"

// Prep for turning off cellular, getting battery details, and incrementing/toggling
SYSTEM_MODE(MANUAL);
FuelGauge batteryMonitor; // keep an eye on Particle's battery
int count = 0;
int led1 = D6;
bool led_state = LOW;

 // Pick an SPI configuration.
 // See SPI configuration section below (comments are for photon).
 #define SPI_CONFIGURATION 0
 //------------------------------------------------------------------------------
 // Setup SPI configuration.
 #if SPI_CONFIGURATION == 0
 // Primary SPI with DMA
 // SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
 SdFat sd;
 const uint8_t chipSelect = SS;
 #endif  // SPI_CONFIGURATION

 //------------------------------------------------------------------------------

 File myFile;

 void setup() {
   Cellular.off();
   pinMode(led1, OUTPUT);
   Serial.begin(9600);
   delay(5000);
   // Wait for USB Serial
   while (!Serial) {
     SysCall::yield();
   }

   Serial.println("Type any character to start");
   while (Serial.read() <= 0) {
     SysCall::yield();
   }

   // Initialize SdFat or print a detailed error message and halt
   // Use half speed like the native library.
   // Change to SPI_FULL_SPEED for more performance.
   if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
     sd.initErrorHalt();
   }

   // open the file for write at end like the "Native SD library"
   if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
     sd.errorHalt("opening test.txt for write failed");
   }
   // if the file opened okay, write to it:
   Serial.print("Writing to test.txt...");
   myFile.println("testing 1, 2, 3.");
   myFile.printf("fileSize: %d\n", myFile.fileSize());

   // close the file:
   myFile.close();
   Serial.println("done.");

   // re-open the file for reading:
   if (!myFile.open("test.txt", O_READ)) {
     sd.errorHalt("opening test.txt for read failed");
   }
   Serial.println("test.txt content:");

   // read from the file until there's nothing else in it:
   int data;
   while ((data = myFile.read()) >= 0) {
     Serial.write(data);
   }
   // close the file:
   myFile.close();

   // Shut off serial port in hopes that it will prevent breaking stuff
   delay(5000); // give serial.write enough time to print out everything
   Serial.end();
 }

 void loop() {
   // Create vars for battery params and buffer for SD card
   float cellVoltage = batteryMonitor.getVCell();
   float stateOfCharge = batteryMonitor.getSoC();
   char buf[100];

   // open the file for write at end like the "Native SD library"
   if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
     sd.errorHalt("opening test.txt for write failed");
   }
   // Write into buffer the increment and battery params
   snprintf(buf, sizeof(buf), "%d, %.02f, %.02f", count, cellVoltage, stateOfCharge);
   myFile.println(buf);

   myFile.close();
   
   // Toggle LED, increment counter, delay 100 ms
   digitalWrite(led1, !led_state);
   count++;
   delay(100);
 }
