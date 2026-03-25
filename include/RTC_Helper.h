#ifndef RTC_HELPER_H
#define RTC_HELPER_H

#include <ThreeWire.h>  
#include <RtcDS1302.h>

//define Pins
#define RTC_IO 26
#define RTC_SCLK 25
#define RTC_CE 27

ThreeWire myWire(RTC_IO, RTC_SCLK, RTC_CE); 
RtcDS1302<ThreeWire> Rtc(myWire);

void initRTC() {
    Rtc.Begin();

    //check if the RTC is running (if the battery died, it stops)
    if (!Rtc.GetIsRunning()) {
        Serial.println("RTC was not actively running, starting now...");
        Rtc.SetIsRunning(true);
    }

    // This sets the RTC to the time this code was compiled
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    Rtc.SetDateTime(compiled);
    Serial.println("RTC Initialized and Time Set.");
}

String getTimestamp() {
    RtcDateTime now = Rtc.GetDateTime();
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            now.Month(), now.Day(), now.Year(),
            now.Hour(), now.Minute(), now.Second());
    
    return String(datestring);
}

#endif