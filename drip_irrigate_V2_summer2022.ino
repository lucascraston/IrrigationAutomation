//Lucas Craston Summer 2022
//software for irrigation timer version 2
//microncontroller is asleep until alarm interupt from RTC, then decides to water or not.

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <DS3232RTC.h>             
#include <Time.h>                  
#include <TinyWireM.h>             

   
DS3232RTC myRTC;

bool alarm_signal;
int alarm_date;


void setup() {
    
    pinMode(3,OUTPUT);
    pinMode(1,INPUT);
 
    //pinMode(1,INPUT_PULLUP)
    myRTC.begin();
    //set alarm regs to a known state
    myRTC.setAlarm(DS3232RTC::ALM1_MATCH_DATE, 0, 0, 0, 1);
    myRTC.setAlarm(DS3232RTC::ALM2_MATCH_DATE, 0, 0, 0, 1);
    myRTC.alarm(DS3232RTC::ALARM_1);
    myRTC.alarm(DS3232RTC::ALARM_2);
    myRTC.alarmInterrupt(DS3232RTC::ALARM_1, false);
    myRTC.alarmInterrupt(DS3232RTC::ALARM_2, false);
    myRTC.squareWave(DS3232RTC::SQWAVE_NONE);


    //use this to set the rtc to a known time      
    /*
    tmElements_t tm;  
    tm.Hour = 10;
    tm.Minute = 15;
    tm.Second = 00;
    tm.Year = 2022-1970;
    tm.Month = 6;
    tm.Day = 18;
    
    myRTC.write(tm);
    */
    //this is our desired alarm time
    // set Alarm 2 for 06:30:00
    myRTC.setAlarm(DS3232RTC::ALM2_MATCH_HOURS, 0, 30, 5, 0);
    // clear the alarm flags
    myRTC.alarm(DS3232RTC::ALARM_1);
    myRTC.alarm(DS3232RTC::ALARM_2);
    // configure the INT/SQW pin for "interrupt" operation (i.e. disable square wave output)
    myRTC.squareWave(DS3232RTC::SQWAVE_NONE);
    // enable interrupt output for Alarm 2 only
    myRTC.alarmInterrupt(DS3232RTC::ALARM_1, false);
    myRTC.alarmInterrupt(DS3232RTC::ALARM_2, true);
    enable_interupt();
    enter_sleep();
       
}

void enable_interupt(){
    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT1); 
  
}

void enter_sleep(){
    
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement
    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep
}

ISR(PCINT0_vect) {
 alarm_signal = true;   
 }


void loop() {
/*
 * water for 10, rest for 20, water for another 10
 * this can be changed by the user and I frequently updated based off 
 * time of year
 * 
 */
  
  cli();
  time_t time_now = myRTC.get();
  if(alarm_signal == true && myRTC.checkAlarm(DS3232RTC::ALARM_2) && (day(time_now)%2 ==0)){
    
    digitalWrite(3,HIGH);
  
    
    while(minute(time_now) != 40)
    {
      time_now = myRTC.get();
      
    }
    digitalWrite(3,LOW);
    while(hour(time_now)!=6)
    {
      time_now = myRTC.get();
    }
    digitalWrite(3,HIGH);
    while(minute(time_now)!=10)
    {
      time_now = myRTC.get();
    }
    digitalWrite(3,LOW);
  }
  alarm_signal = false;
  enter_sleep();
  
}
