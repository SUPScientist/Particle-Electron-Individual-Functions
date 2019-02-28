/*
 * Project UBlox4_SimpleNMEA_UART
 * Description:
 * Author:
 * Date:
 */

SYSTEM_MODE(MANUAL);

void setup() {
  // Redundantly turn off cell modem to ensure no data transfer
  Cellular.off();

  // Turn on serial comms with PC and with peripheral (UBloxM8Q)
  Serial.begin(9600);
  Serial1.begin(9600);

  // Wait a few secs then let us know we're awake
  delay(5000);
  Serial.println("Enabling GPS...");

}

void loop() {
  nemaMsgEnable("GSV");
  delay(2000);
}

// From SO: https://gis.stackexchange.com/questions/198846/enabling-disabling-nmea-sentences-on-u-blox-gps-receiver
int calculateChecksum(const char *msg)
{
    int checksum = 0;
    for (int i = 0; msg[i] && i < 32; i++)
        checksum ^= (unsigned char)msg[i];

    return checksum;
}

int nemaMsgSend(const char *msg)
{
    char checksum[8];
    snprintf(checksum, sizeof(checksum)-1, F("*%.2X"), calculateChecksum(msg));
    Serial1.print("$");
    Serial1.print(msg);
    Serial1.println(checksum);
}

int nemaMsgDisable(const char *nema)
{
    if (strlen(nema) != 3) return 0;

    char tmp[32];
    snprintf(tmp, sizeof(tmp)-1, "PUBX,40,%s,0,0,0,0", nema);
    //snprintf(tmp, sizeof(tmp)-1, F("PUBX,40,%s,0,0,0,0,0,0"), nema);
    nemaMsgSend(tmp);

    return 1;
}

int nemaMsgEnable(const char *nema)
{
    if (strlen(nema) != 3) return 0;

    char tmp[32];
    snprintf(tmp, sizeof(tmp)-1, "PUBX,40,%s,0,1,0,0", nema);
    //snprintf(tmp, sizeof(tmp)-1, F("PUBX,40,%s,0,1,0,0,0,0"), nema);
    nemaMsgSend(tmp);

    return 1;
}
