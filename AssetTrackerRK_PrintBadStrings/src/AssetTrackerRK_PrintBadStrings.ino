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
#include "Serial5/Serial5.h"
#define PWR_3V3SW_EN_PIN  D6

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

	// The GPS module on the AssetTracker is connected to Serial1 and D6
	Serial5.begin(9600);

	// For Smartfin Z5, turn on 3V3SW to use GPS
  pinMode(PWR_3V3SW_EN_PIN, OUTPUT);
  digitalWrite(PWR_3V3SW_EN_PIN, HIGH);
   startFix = millis();
   gettingFix = true;
}

void loop()
{
	while (Serial5.available() > 0) {
    Serial.write(Serial5.read());
    if (gps.encode(Serial5.read())) {
			displayInfo();
		}
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
