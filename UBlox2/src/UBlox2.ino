// hold off connecting to cell/netowrk/cloud. See below.
SYSTEM_MODE(MANUAL)

#include "AssetTracker2.h"

// Used to keep track of the last time we published data
long lastPublish = 0;

// Minutes between publishing
unsigned int delayMinutes = 1;

// Creating an AssetTracker named 't' for us to reference
AssetTracker2 t = AssetTracker2();

// A FuelGauge named 'fuel' for checking on the battery state
FuelGauge fuel;

String gpsPosition = "Not fixed";

// All the data in one string
// "lat, lon, acc, alt, spd, batV, bat%"
String buf;

int batteryStatus(String command);

void setup() {
  Cellular.off();
    // Opens up a Serial port so you can listen over USB
    Serial.begin(9600);

    // Wait to allow particle serial monitor to get connected
    delay(10000);
    Serial.println("Starting up!");

    // enabling gps while cell service is connecting doesn't work because
    // of conflict for cpu time.
    // Turn on gps then turn on cell service. Doing it in the reverse
    // order doesn't work

    // Enable the GPS module.
    t.gpsOn();

}

void loop() {

    // You'll need to run this every loop to capture the GPS output
    Serial.println("Attempting GPS fix");
    t.updateGPS();


    // is it time to publish?
    if (millis() - lastPublish > delayMinutes*60*1000) {
        // Remember when we published
        lastPublish = millis();

        //String pubAccel = String::format("%d,%d,%d", t.readX(), t.readY(), t.readZ());
        //Serial.println(pubAccel);
        //Particle.publish("A", pubAccel, 60, PRIVATE);

        Serial.println(gpsPosition);

        // we should only publish data if there's a fix
        if (t.gpsFix()) {
            // report the data over serial
            Serial.println(t.readLatLon());
        }
    }
}


// Allows changing the measurement rate
int gpsRate(String command) {
    uint16_t rate = atoi(command);
    int nav = atoi(command.substring(command.indexOf(' ')));
    Serial.print("rate: ");
    Serial.print(rate);
    Serial.print(" nav: ");
    Serial.println(nav);
    t.gpsRate(rate, nav);
    return 1;
}


// Lets you remotely check the battery status by calling the function "batt"
// Triggers a publish with the info (so subscribe or watch the dashboard)
// and also returns a '1' if there's >10% battery left and a '0' if below
int batteryStatus(String command){
    // if there's more than 10% of the battery left, then return 1
    if (fuel.getSoC()>10){ return 1;}
    // if you're running out of battery, return 0
    else { return 0;}
}
