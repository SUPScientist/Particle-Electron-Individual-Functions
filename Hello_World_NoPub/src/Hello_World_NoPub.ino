/*
 * Project Hello_World_NoPub
 * Description:
 * Author:
 * Date:
 */

SYSTEM_MODE(MANUAL);

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Cellular.off();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  Serial.println("Hello, world");
  delay(5000);
}
