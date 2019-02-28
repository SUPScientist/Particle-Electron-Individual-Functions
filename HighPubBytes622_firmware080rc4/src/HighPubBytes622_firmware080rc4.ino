// from https://github.com/particle-iot/firmware/pull/1537

 #include "Particle.h"

 #define PUB_70 "123456789012345678901234567890123456789012345678901234567890ABCDEFGHIJ"
 #define SUB_70 "123456789012345678901234567890123456789012345678901234567890ABCDEFGHIJ"
 #define VAR_70 "123456789012345678901234567890123456789012345678901234567890ABCDEFGHIJ"
 #define VAR_64 "123456789012345678901234567890123456789012345678901234567890ABCD"
 #define VAR_63 "123456789012345678901234567890123456789012345678901234567890ABC"
 #define VAR_62 "123456789012345678901234567890123456789012345678901234567890AB"
 #define VAR_61 "123456789012345678901234567890123456789012345678901234567890A"
 #define VAR_60 "123456789012345678901234567890123456789012345678901234567890"
 #define VAR_13 "1234567890123"
 #define VAR_12 "123456789012"
 #define VAR_OVF "12345678901234567890123456789012345678901234567890"

 #define FUN_70 "123456789012345678901234567890123456789012345678901234567890ABCDEFGHIJ"
 #define FUN_64 "123456789012345678901234567890123456789012345678901234567890ABCD"
 #define FUN_63 "123456789012345678901234567890123456789012345678901234567890ABC"
 #define FUN_62 "123456789012345678901234567890123456789012345678901234567890AB"
 #define FUN_61 "123456789012345678901234567890123456789012345678901234567890A"
 #define FUN_60 "123456789012345678901234567890123456789012345678901234567890"
 #define FUN_13 "1234567890123"
 #define FUN_12 "123456789012"

 #define CHAR_700 "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890ABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJ"
 #define CHAR_622 "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890AB"

 String my_variable_ovf = CHAR_622;
 String my_variable_12 = CHAR_622;
 String my_variable_13 = CHAR_622;
 String my_variable_60 = CHAR_622;
 String my_variable_61 = CHAR_622;
 String my_variable_62 = CHAR_622;
 String my_variable_63 = CHAR_622;
 String my_variable_64 = CHAR_622;
 String my_variable_70 = CHAR_700;

 Serial1LogHandler logHandler(115200, LOG_LEVEL_ALL);

 void handler(const char *event, const char *data)
 {
     String my_event(event);
     String my_data(data);
     Serial.printlnf("Event Length: %d, Data Length: %d", my_event.length(), my_data.length() );
     Serial.printlnf("Event Name: %s, Event Data: %s", my_event.c_str(), my_data.c_str() );
 }

 void printInfo(String& string) {
     Serial.printlnf("Function Argument Length: %d", string.length() );
     Serial.printlnf("Function Argument: %s", string.c_str() );
 }
 int my_function_70(String arg) {
     printInfo(arg);
     return 0;
 }
 int my_function_64(String arg) {
     printInfo(arg);
     return 0;
 }
 int my_function_63(String arg) {
     printInfo(arg);
     return 0;
 }
 int my_function_62(String arg) {
     printInfo(arg);
     return 0;
 }
 int my_function_61(String arg) {
     printInfo(arg);
     return 0;
 }
 int my_function_60(String arg) {
     printInfo(arg);
     return 0;
 }
 int my_function_50(String arg) {
     printInfo(arg);
     return 0;
 }
 int my_function_13(String arg) {
     printInfo(arg);
     return 0;
 }
 int my_function_12(String arg) {
     printInfo(arg);
     return 0;
 }
 int my_function_ovf(String arg) {
     printInfo(arg);
     return 0;
 }

 void setup() {
     Serial.begin();

     // Particle.function(FUN_70, my_function_70); // error on compile (error shows 64 limit)
     Particle.function(FUN_12, my_function_12); // can receive 622 bytes as an argument
     Particle.function(FUN_13, my_function_13); // can receive 622 bytes as an argument
     Particle.function(FUN_60, my_function_60); // can receive 622 bytes as an argument
     Particle.function(FUN_61, my_function_61); // can receive 622 bytes as an argument
     Particle.function(FUN_62, my_function_62); // can receive 622 bytes as an argument
     Particle.function(FUN_63, my_function_63); // can receive 622 bytes as an argument
     Particle.function(FUN_64, my_function_64); // can receive 622 bytes as an argument

     // Particle.variable(VAR_70, &my_variable_70, STRING); // error on compile (error shows 64 limit)
     Particle.variable(VAR_12, &my_variable_12, STRING); // returns CHAR_622
     Particle.variable(VAR_13, &my_variable_13, STRING); // returns CHAR_622
     Particle.variable(VAR_60, &my_variable_60, STRING); // returns CHAR_622
     Particle.variable(VAR_61, &my_variable_61, STRING); // returns CHAR_622
     Particle.variable(VAR_62, &my_variable_62, STRING); // returns CHAR_622
     Particle.variable(VAR_63, &my_variable_63, STRING); // returns CHAR_622
     Particle.variable(VAR_64, &my_variable_64, STRING); // returns CHAR_622
     // Particle.variable(VAR_OVF, &my_variable_ovf, STRING); // add to increase describe message over the limit

     Particle.subscribe(SUB_70, handler, MY_DEVICES); // event 64 and data 255 (limited by Cloud API)
     waitUntil(Particle.connected);

     Particle.publish(PUB_70, CHAR_700, PRIVATE, WITH_ACK); // event 64 and data 622
 }

 void loop() {

 }
