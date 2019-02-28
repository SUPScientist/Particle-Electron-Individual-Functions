/*
 * Project UBlox3_SerialNMEA
 * Description:
 * Author:
 * Date:
 */

// setup() runs once, when the device is first turned on.

SYSTEM_MODE(MANUAL); // so we can turn off cellular modem

unsigned long lastTime = 0;

void setup() {
  Cellular.off();
	Serial.begin(9600);
	Serial1.begin(9600);
}

void loop() {
	while (Serial1.available()) {
		Serial.write(Serial1.read());
    Serial.println("Hey!");
	}

	if (millis() - lastTime >= 1000) {
		lastTime = millis();
		Serial.println("called once per second");
	}
}
