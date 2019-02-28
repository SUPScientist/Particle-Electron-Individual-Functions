/*
 * Project PinLow
 * Description:
 * Author:
 * Date:
 */

int TEST_PIN = B0;
int sw3v3 = B5;

SYSTEM_MODE(MANUAL);
bool pinState = false;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Cellular.off();
  pinMode(TEST_PIN, OUTPUT);
  digitalWrite(TEST_PIN, pinState);

  pinMode(sw3v3, OUTPUT);
  digitalWrite(sw3v3, HIGH);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  delay(5000);
  pinState = !pinState;
  digitalWrite(TEST_PIN, pinState);
}
