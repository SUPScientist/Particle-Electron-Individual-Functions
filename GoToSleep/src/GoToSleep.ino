SYSTEM_MODE(MANUAL);

void setup() {
  Cellular.off();
  delay(5000);
}

void loop() {
  Serial.println("Awake");
  delay(5000);
  System.sleep(SLEEP_MODE_DEEP, 20);
}
