/*
 * Project Hello_World_Publish
 * Description:
 * Author:
 * Date:
 */

const char *eventName = "Sfin-Z5-00001";

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Particle.publish(eventName, "Hello, world!", 60, PRIVATE);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

}
