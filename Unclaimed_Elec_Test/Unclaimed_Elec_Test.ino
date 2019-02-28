/*
 * Project Basic_Serial1_Read_Write
 * Description:
 * Author:
 * Date:
 */
SYSTEM_MODE(MANUAL);
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  Serial1.begin(9600);
  Cellular.off();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  Serial.println("loop repeating");
  Serial1.write(0xC1);
  delay(2000);
}
