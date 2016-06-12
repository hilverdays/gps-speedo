#include <AD9850.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;

const int W_CLK_PIN = 8;
const int FQ_UD_PIN = 9;
const int DATA_PIN = 10;
const int RESET_PIN = 11;

double freq = 1;
double trimFreq = 124999500;

int phase = 0;

unsigned int speed = 0;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  DDS.begin(W_CLK_PIN, FQ_UD_PIN, DATA_PIN, RESET_PIN);
  DDS.calibrate(trimFreq);
}

void loop()
{
  // Dispatch incoming characters
  while (ss.available() > 0)
    gps.encode(ss.read());


  if (gps.speed.isUpdated())
  {
    Serial.print(F("SPEED      Fix Age="));
    Serial.print(gps.speed.age());
    Serial.print(F("ms Raw="));
    Serial.print(gps.speed.value());
    Serial.print(F(" km/h="));
    Serial.println(gps.speed.kmph()); 
    speed=gps.speed.kmph();
    //calculate the frequency with map()
    freq = map(speed, 0, 100, 0, 160);
    DDS.setfreq(freq, phase);   
  }

}

