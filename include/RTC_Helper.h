#ifndef RTC_HELPER_H
#define RTC_HELPER_H

#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <sys/time.h>

// Pins from your groupmate's code
ThreeWire myWire(26, 25, 27); 
RtcDS1302<ThreeWire> Rtc(myWire);

void initRTC() {
    Rtc.Begin();
    if (!Rtc.GetIsRunning()) Rtc.SetIsRunning(true);
    Rtc.SetIsWriteProtected(false);
}

// Fixes the 1990 date issue by syncing RTC to ESP32 system time
void syncSystemTime() {
    RtcDateTime now = Rtc.GetDateTime();
    struct tm tm;
    tm.tm_year = now.Year() - 1900;
    tm.tm_mon = now.Month() - 1;
    tm.tm_mday = now.Day();
    tm.tm_hour = now.Hour();
    tm.tm_min = now.Minute();
    tm.tm_sec = now.Second();
    time_t t = mktime(&tm);
    struct timeval now_tv = { .tv_sec = t };
    settimeofday(&now_tv, NULL);
}
#endif