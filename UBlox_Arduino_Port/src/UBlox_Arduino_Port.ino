/*
 * Project UBlox_Arduino_Port
 * Description:
 * Author:
 * Date:
 */

 boolean gpsStatus[] = {false, false, false, false, false, false, false};
 unsigned long start;

 void setup()
 {
   Serial1.begin(9600); // GPS

   // START OUR SERIAL DEBUG PORT
   Serial.begin(115200);
   //
   //Settings Array contains the following settings: [0]NavMode, [1]DataRate1, [2]DataRate2, [3]PortRatebyte1, [4]PortRatebyte2, [5]PortRatebyte3,
   //[6]NMEA GLL Sentence, [7]NMEA GSA Sentence, [8]NMEA GSV Sentence, [9]NMEA RMC Sentence, [10]NMEA VTG Sentence
   //NavMode:
   //Pedestrian Mode    = 0x03
   //Automotive Mode    = 0x04
   //Sea Mode           = 0x05
   //Airborne < 1G Mode = 0x06
   //
   //DataRate:
   //1Hz     = 0xE8 0x03
   //2Hz     = 0xF4 0x01
   //3.33Hz  = 0x2C 0x01
   //4Hz     = 0xFA 0x00
   //
   //PortRate:
   //4800   = C0 12 00
   //9600   = 80 25 00
   //19200  = 00 4B 00  **SOFTWARESERIAL LIMIT FOR ARDUINO UNO R3!**
   //38400  = 00 96 00  **SOFTWARESERIAL LIMIT FOR ARDUINO MEGA 2560!**
   //57600  = 00 E1 00
   //115200 = 00 C2 01
   //230400 = 00 84 03
   //
   //NMEA Messages:
   //OFF = 0x00
   //ON  = 0x01
   //
   byte settingsArray[] = {0x03, 0xFA, 0x00, 0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //
   configureUblox(settingsArray);
 }

 void loop()
 {
   while(1) {
     if(Serial1.available()) {
       // THIS IS THE MAIN LOOP JUST READS IN FROM THE GPS SERIAL AND ECHOS OUT TO THE ARDUINO SERIAL.
       Serial.println("anything");
       Serial.write(Serial1.read());
     }
   }
 }


 void configureUblox(byte *settingsArrayPointer) {
   byte gpsSetSuccess = 0;
   Serial.println("Configuring u-Blox GPS initial state...");

   //Generate the configuration string for Navigation Mode
   byte setNav[] = {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, *settingsArrayPointer, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
   calcChecksum(&setNav[2], sizeof(setNav) - 4);

   //Generate the configuration string for Data Rate
   byte setDataRate[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, settingsArrayPointer[1], settingsArrayPointer[2], 0x01, 0x00, 0x01, 0x00, 0x00, 0x00};
   calcChecksum(&setDataRate[2], sizeof(setDataRate) - 4);

   //Generate the configuration string for Baud Rate
   byte setPortRate[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, settingsArrayPointer[3], settingsArrayPointer[4], settingsArrayPointer[5], 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
   calcChecksum(&setPortRate[2], sizeof(setPortRate) - 4);

   byte setGLL[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B};
   byte setGSA[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32};
   byte setGSV[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39};
   byte setRMC[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x40};
   byte setVTG[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x46};

   delay(2500);

   while(gpsSetSuccess < 3)
   {
     Serial.print("Setting Navigation Mode... ");
     sendUBX(&setNav[0], sizeof(setNav));  //Send UBX Packet
     gpsSetSuccess += getUBX_ACK(&setNav[2]); //Passes Class ID and Message ID to the ACK Receive function
     if (gpsSetSuccess == 5) {
       gpsSetSuccess -= 4;
       setBaud(settingsArrayPointer[4]);
       delay(1500);
       byte lowerPortRate[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA2, 0xB5};
       sendUBX(lowerPortRate, sizeof(lowerPortRate));
       Serial1.begin(9600);
       delay(2000);
     }
     if(gpsSetSuccess == 6) gpsSetSuccess -= 4;
     if (gpsSetSuccess == 10) gpsStatus[0] = true;
   }
   if (gpsSetSuccess == 3) Serial.println("Navigation mode configuration failed.");
   gpsSetSuccess = 0;
   while(gpsSetSuccess < 3) {
     Serial.print("Setting Data Update Rate... ");
     sendUBX(&setDataRate[0], sizeof(setDataRate));  //Send UBX Packet
     gpsSetSuccess += getUBX_ACK(&setDataRate[2]); //Passes Class ID and Message ID to the ACK Receive function
     if (gpsSetSuccess == 10) gpsStatus[1] = true;
     if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
   }
   if (gpsSetSuccess == 3) Serial.println("Data update mode configuration failed.");
   gpsSetSuccess = 0;


   while(gpsSetSuccess < 3 && settingsArrayPointer[6] == 0x00) {
     Serial.print("Deactivating NMEA GLL Messages ");
     sendUBX(setGLL, sizeof(setGLL));
     gpsSetSuccess += getUBX_ACK(&setGLL[2]);
     if (gpsSetSuccess == 10) gpsStatus[2] = true;
     if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
   }
   if (gpsSetSuccess == 3) Serial.println("NMEA GLL Message Deactivation Failed!");
   gpsSetSuccess = 0;

   while(gpsSetSuccess < 3 && settingsArrayPointer[7] == 0x00) {
     Serial.print("Deactivating NMEA GSA Messages ");
     sendUBX(setGSA, sizeof(setGSA));
     gpsSetSuccess += getUBX_ACK(&setGSA[2]);
     if (gpsSetSuccess == 10) gpsStatus[3] = true;
     if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
   }
   if (gpsSetSuccess == 3) Serial.println("NMEA GSA Message Deactivation Failed!");
   gpsSetSuccess = 0;

   while(gpsSetSuccess < 3 && settingsArrayPointer[8] == 0x00) {
     Serial.print("Deactivating NMEA GSV Messages ");
     sendUBX(setGSV, sizeof(setGSV));
     gpsSetSuccess += getUBX_ACK(&setGSV[2]);
     if (gpsSetSuccess == 10) gpsStatus[4] = true;
     if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
   }
   if (gpsSetSuccess == 3) Serial.println("NMEA GSV Message Deactivation Failed!");
   gpsSetSuccess = 0;

   while(gpsSetSuccess < 3 && settingsArrayPointer[9] == 0x00) {
     Serial.print("Deactivating NMEA RMC Messages ");
     sendUBX(setRMC, sizeof(setRMC));
     gpsSetSuccess += getUBX_ACK(&setRMC[2]);
     if (gpsSetSuccess == 10) gpsStatus[5] = true;
     if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
   }
   if (gpsSetSuccess == 3) Serial.println("NMEA RMC Message Deactivation Failed!");
   gpsSetSuccess = 0;

   while(gpsSetSuccess < 3 && settingsArrayPointer[10] == 0x00) {
     Serial.print("Deactivating NMEA VTG Messages ");
     sendUBX(setVTG, sizeof(setVTG));
     gpsSetSuccess += getUBX_ACK(&setVTG[2]);
     if (gpsSetSuccess == 10) gpsStatus[6] = true;
     if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
   }
   if (gpsSetSuccess == 3) Serial.println("NMEA VTG Message Deactivation Failed!");

   gpsSetSuccess = 0;
   if (settingsArrayPointer[4] != 0x25) {
     Serial.print("Setting Port Baud Rate... ");
     sendUBX(&setPortRate[0], sizeof(setPortRate));
     setBaud(settingsArrayPointer[4]);
     Serial.println("Success!");
     delay(500);
   }
 }

 // Extra smaller functions to work with UBX proprietary data
 void calcChecksum(byte *checksumPayload, byte payloadSize) {
   byte CK_A = 0, CK_B = 0;
   for (int i = 0; i < payloadSize ;i++) {
     CK_A = CK_A + *checksumPayload;
     CK_B = CK_B + CK_A;
     checksumPayload++;
   }
   *checksumPayload = CK_A;
   checksumPayload++;
   *checksumPayload = CK_B;
 }

 void sendUBX(byte *UBXmsg, byte msgLength) {
   for(int i = 0; i < msgLength; i++) {
     Serial1.write(UBXmsg[i]);
     Serial1.flush();
   }
   Serial1.println();
   Serial1.flush();
 }


 byte getUBX_ACK(byte *msgID) {
   byte CK_A = 0, CK_B = 0;
   byte incoming_byte;
   boolean headerReceived = false;
   unsigned long ackWait = millis();
   byte ackPacket[10] = {0xB5, 0x62, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
   int i = 0;
   while (1) {
    //  if (Serial1.available()) {
      //  Serial.println("Serial1 avail");
       incoming_byte = Serial1.read();
       if (incoming_byte == ackPacket[i]) {
         i++;
       }
       else if (i > 2) {
         ackPacket[i] = incoming_byte;
         i++;
       }
    //  }
     if (i > 9) break;
     if ((millis() - ackWait) > 1500) {
       Serial.println("ACK Timeout");
       return 5;
     }
     if (i == 4 && ackPacket[3] == 0x00) {
       Serial.println("NAK Received");
       return 1;
     }
   }

   for (i = 2; i < 8 ;i++) {
   CK_A = CK_A + ackPacket[i];
   CK_B = CK_B + CK_A;
   }
   if (msgID[0] == ackPacket[6] && msgID[1] == ackPacket[7] && CK_A == ackPacket[8] && CK_B == ackPacket[9]) {
     Serial.println("Success!");
     Serial.print("ACK Received! ");
     printHex(ackPacket, sizeof(ackPacket));
     return 10;
         }
   else {
     Serial.print("ACK Checksum Failure: ");
     printHex(ackPacket, sizeof(ackPacket));
     delay(1000);
     return 1;
   }
 }


 void printHex(uint8_t *data, uint8_t length) // prints 8-bit data in hex
 {
   byte tmp[length*2+1];
   byte first ;
   int j=0;
   for (byte i = 0; i < length; i++)
   {
     first = (data[i] >> 4) | 48;
     if (first > 57) tmp[j] = first + (byte)7;
     else tmp[j] = first ;
     j++;

     first = (data[i] & 0x0F) | 48;
     if (first > 57) tmp[j] = first + (byte)7;
     else tmp[j] = first;
     j++;
   }
   tmp[length*2] = 0;
   for (byte i = 0, j = 0; i < sizeof(tmp); i++) {
     Serial.print(tmp[i]);
     if (j == 1) {
       Serial.print(" ");
       j = 0;
     }
     else j++;
   }
   Serial.println();
 }

 void setBaud(byte baudSetting) {
   if (baudSetting == 0x12) Serial1.begin(4800);
   if (baudSetting == 0x4B) Serial1.begin(19200);
   if (baudSetting == 0x96) Serial1.begin(38400);
   if (baudSetting == 0xE1) Serial1.begin(57600);
   if (baudSetting == 0xC2) Serial1.begin(115200);
   if (baudSetting == 0x84) Serial1.begin(230400);
 }
