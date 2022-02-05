#include "DatetimeInterval.h"

DatetimeInterval::DatetimeInterval(NTPClient ntpClient) : ntpClient(ntpClient) {
}

void DatetimeInterval::begin(uint8_t totalMinuteUpdate) {
    if (totalMinuteUpdate > 240) {
        totalMinuteUpdate = 240;
    }

    /* Calculating the datetime for next update about RTC. */
    timespanDatetimeRTC = TimeSpan(TIMEOUT_CHECK_RTC_DAY, 0, 0, 0);
    updateDatetimeRTC();
    configNextDatetimeRTC();

    /* Calculating the datetime for next update. */
    uint8_t updateHour = totalMinuteUpdate / 60;
    uint8_t updateMinute = totalMinuteUpdate - (updateHour * 60);
    timespanDatetime = TimeSpan(0, updateHour, updateMinute, 0);
    configNextDatetime();
}

bool DatetimeInterval::checkDatetime() {
    /* Checking the datetime of RTC for updating if is necessary. */
    if (checkDatetimeRTC()) {
        updateDatetimeRTC();
        configNextDatetimeRTC();
    }

    return DateTime(rtc.now()) > nextDatetime;
}

uint16_t DatetimeInterval::getActualYear() { return DateTime(rtc.now()).year(); }

uint8_t DatetimeInterval::getActualMonth() { return DateTime(rtc.now()).month(); }

uint8_t DatetimeInterval::getActualDay() { return DateTime(rtc.now()).day(); }

uint8_t DatetimeInterval::getActualDayWeek() { return DateTime(rtc.now()).dayOfTheWeek(); }

uint8_t DatetimeInterval::getActualHour() { return DateTime(rtc.now()).hour(); }

uint8_t DatetimeInterval::getActualMinute() { return DateTime(rtc.now()).minute(); }

uint8_t DatetimeInterval::getActualSecond() { return DateTime(rtc.now()).second(); }

struct tm DatetimeInterval::getTmDatetime(DateTime &datetime) {
    struct tm actualDatetime_tm{};
    actualDatetime_tm.tm_year = datetime.year();
    actualDatetime_tm.tm_mon = datetime.month();
    actualDatetime_tm.tm_mday = datetime.day();
    actualDatetime_tm.tm_wday = datetime.dayOfTheWeek();
    actualDatetime_tm.tm_hour = datetime.hour();
    actualDatetime_tm.tm_min = datetime.minute();
    actualDatetime_tm.tm_sec = datetime.second();

    return actualDatetime_tm;
}

String DatetimeInterval::getActualTimestamp() {
    char timestamp[26];

    DateTime actualDatetime = DateTime(rtc.now());
    struct tm actualDatetime_tm = getTmDatetime(actualDatetime);

    sprintf(
            timestamp,
            "%4d-%02d-%02d %02d:%02d:%02d",
            actualDatetime_tm.tm_year,
            actualDatetime_tm.tm_mon,
            actualDatetime_tm.tm_mday,
            actualDatetime_tm.tm_hour,
            actualDatetime_tm.tm_min,
            actualDatetime_tm.tm_sec
    );

    return String(timestamp);
}

void DatetimeInterval::configNextDatetime() {
    String daysOfTheWeek[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

    DateTime actualDatetime = DateTime(rtc.now());
    struct tm actualDatetime_tm = getTmDatetime(actualDatetime);

    nextDatetime = DateTime(actualDatetime + timespanDatetime);

    Serial.println("\033[1;92m---------------------- [TIME] ---------------------\033[0m");
    Serial.print("\t\033[1;97mACTUAL: ");
    Serial.print(daysOfTheWeek[actualDatetime_tm.tm_wday]); Serial.print(" - ");
    Serial.printf("%d-%02d-%02d %02d:%02d:%02d", actualDatetime_tm.tm_year, actualDatetime_tm.tm_mon, actualDatetime_tm.tm_mday, actualDatetime_tm.tm_hour, actualDatetime_tm.tm_min, actualDatetime_tm.tm_sec);
    Serial.println("\033[0m");
    Serial.print("\t\033[1;97mNEXT:   ");
    Serial.print(daysOfTheWeek[nextDatetime.dayOfTheWeek()]); Serial.print(" - ");
    Serial.printf("%d-%02d-%02d %02d:%02d:%02d", nextDatetime.year(), nextDatetime.month(), nextDatetime.day(), nextDatetime.hour(), nextDatetime.minute(), nextDatetime.second());
    Serial.println("\033[0m");
    Serial.println("\033[1;92m---------------------------------------------------\033[0m\n");
}

bool DatetimeInterval::checkDatetimeRTC() {
    return DateTime(rtc.now()) > nextDatetimeRTC;
}

void DatetimeInterval::configNextDatetimeRTC() {
    nextDatetimeRTC = DateTime(rtc.now() + timespanDatetimeRTC);
}

void DatetimeInterval::updateDatetimeRTC() {
    /* Connecting to NTP server to get the actual datetime. */
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    ntpClient.begin();
    while (!ntpClient.update()) {
        yield();

        Serial.println("\033[1;91m[NTP ERROR]\033[0m");
        delay(TIMEOUT_CHECK_NTP_MILLISECONDS);
    }

    /* Updating the RTC. */
    rtc.adjust(DateTime(ntpClient.getEpochTime()));
    Serial.println("\033[1;92m[RTC UPDATED]\033[0m");

    ntpClient.end();
}