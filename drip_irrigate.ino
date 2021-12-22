
#include <DS3231.h>

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);





int Relay = 3;



Time t;

void setup()
{
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);
  Serial.begin(115200);
  rtc.begin();
}

void loop()
{
  Serial.print("Current Unixtime.........................: ");
  Serial.println(rtc.getUnixTime(rtc.getTime())); 
  
  t.hour = 5;
  t.min = 0;
  t.sec = 0;
  t.year = 2021;
  t.mon = 6;
  t.date = 10;
  unsigned long fixedUnix = rtc.getUnixTime(t);
  Serial.print("fixed Unix:");
  Serial.println(rtc.getUnixTime(t));
  unsigned long currentUnix = rtc.getUnixTime(rtc.getTime());
  unsigned long deltaUnix = (currentUnix - fixedUnix); 
  Serial.print("Delat Unix:");
  Serial.println(deltaUnix);
  
  if (deltaUnix % 172800 == 0)
  {
      
    digitalWrite(Relay, LOW);
    delay(30UL * 60UL * 1000UL);
    digitalWrite(Relay,HIGH);
    delay(30UL * 60UL * 1000UL);
    digitalWrite(Relay, LOW);
    delay(30UL * 60UL * 1000UL);
    digitalWrite(Relay,HIGH);
  }
  
  
  
}
