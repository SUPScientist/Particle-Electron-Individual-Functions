/*
* Project Analog_Read_Test
* Description: Simple single-pin analog reader with serial print to see the
* resulting values. No conversion (yet) to meaningful units.
* Author:
* Date:
*/

int Vg = A0;
int Vsg = A1;             // potentiometer connected to analog pin A0
int val = 0;                    // variable to store the read value

SYSTEM_MODE(MANUAL); // allow us to manually shut off cellular

void setup()
{
   // Note: analogPin pin does not require pinMode()

   Cellular.off(); // for testing
   delay(5000); // give me headstart to open serial monitor after flashing

   // To print to screen over USB-Serial
   Serial.begin(9600);
   Serial.println("Test Analog_Read.ino: pH sensor");
}

void loop()
{
  //  val = analogRead(Vsg);  // read the analogPin

  Serial.print("Vg = ");
  Serial.print(analogRead(Vg)*3.3/4096);

   Serial.print(",\t Vsg = ");
   Serial.print(analogRead(Vsg)*3.3/4096);

   Serial.println("\t V");
   delay(1000);
}
