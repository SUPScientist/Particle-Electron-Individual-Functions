/*
 * Project AssetTrackerRK_Fresh
 * Description:
 * Author:
 * Date:
 */

#include "Particle.h"

// Port of TinyGPS for the Particle AssetTracker
// https://github.com/mikalhart/TinyGPSPlus

#include "TinyGPS++.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

/*
  This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object directly.
*/

void displayInfo(); // forward declaration

const unsigned long PUBLISH_PERIOD = 120000;
const unsigned long SERIAL_PERIOD = 5000;

// The TinyGPS++ object
TinyGPSPlus gps;
unsigned long lastSerial = 0;
unsigned long lastPublish = 0;
unsigned long startFix = 0;
bool gettingFix = false;

void setup()
{
  Cellular.off();
	Serial.begin(9600);

	// Settings D6 LOW powers up the GPS module
   pinMode(D6, OUTPUT);
   digitalWrite(D6, LOW);
   startFix = millis();
   gettingFix = true;

   // The GPS module on the AssetTracker is connected to Serial1 and D6
 	 Serial1.begin(9600);

   // Change Serial1 to 19200 baud
  //  Serial1.println("$PUBX,41,1,0007,0003,19200,0*25"); // speed up baud on Serial1
  //  delay(500);
  //  Serial1.end(); // close port configured @ 9600, reopen @ 19200
  //  delay(500);
  //  Serial1.begin(19200);
}

void loop()
{
	while (Serial1.available() > 0) {
    Serial.write(Serial1.read());
    delay(1);
    // Serial.println(); // add some space after chars read from Serial1
    // Serial.println("Serial1 is available.");
    // delay(1000);
		// if (gps.encode(Serial1.read())) {
		// 	displayInfo();
		// }
	}

}

void displayInfo()
{
	if (millis() - lastSerial >= SERIAL_PERIOD) {
		lastSerial = millis();

		char buf[128];
		if (gps.location.isValid()) {
			snprintf(buf, sizeof(buf), "%f,%f,%f", gps.location.lat(), gps.location.lng(), gps.altitude.meters());
			if (gettingFix) {
				gettingFix = false;
				unsigned long elapsed = millis() - startFix;
				Serial.printlnf("%lu milliseconds to get GPS fix", elapsed);
			}
		}
		else {
			strcpy(buf, "no location");
			if (!gettingFix) {
				gettingFix = true;
				startFix = millis();
			}
		}
		Serial.println(buf);

		// if (Particle.connected()) {
		// 	if (millis() - lastPublish >= PUBLISH_PERIOD) {
		// 		lastPublish = millis();
		// 		Particle.publish("gps", buf);
		// 	}
		// }
	}

}
