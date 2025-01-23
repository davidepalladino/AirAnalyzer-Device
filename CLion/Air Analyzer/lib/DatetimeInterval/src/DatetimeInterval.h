/**
 * @file DatetimeInterval.h
 * @brief Provides functionality to manage time intervals using an RTC module and NTP synchronization.
 *
 * This library manages the time intervals by comparing the actual time stored in the RTC module
 * with the NTP-synchronized time, ensuring periodic updates.
 *
 * Copyright (c) 2025 Davide Palladino.
 * All rights reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 4.0.0
 * @date 23rd January 2025
 */

#ifndef DATETIMEINTERVAL_H
#define DATETIMEINTERVAL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <RTClib.h>
#include <ctime>

/**
 * @class DatetimeInterval
 * @brief Manages time intervals using an RTC module and NTP synchronization.
 */
class DatetimeInterval {
    public:
        /**
         * @brief Constructs a DatetimeInterval object with NTP client and timeout settings.
         * @param ntpClient Reference to the NTPClient object for time synchronization.
         * @param timeoutCheckRtcDay Timeout for RTC daily checks.
         * @param timeoutCheckNtpMilliseconds Timeout for NTP synchronization in milliseconds.
         */
        explicit DatetimeInterval(
            NTPClient ntpClient,
            uint8_t timeoutCheckRtcDay,
            uint16_t timeoutCheckNtpMilliseconds
        );

        /**
         * @brief Initializes the RTC object and sets the next update time.
         * @param totalMinuteUpdate Total minutes between updates (max: 240 minutes).
         * @warning Values above 240 minutes will be capped.
         */
        void begin(uint8_t totalMinuteUpdate);

        /**
         * @brief Checks if the actual datetime exceeds the next scheduled update.
         * @return True if the update interval has elapsed, false otherwise.
         */
        bool checkDatetime();

        /**
         * @brief Configures the next scheduled datetime based on the update interval.
         */
        void configNextDatetime();

        /** @brief Retrieves the current year from the RTC module. */
        uint16_t getActualYear();

        /** @brief Retrieves the current month from the RTC module. */
        uint8_t getActualMonth();

        /** @brief Retrieves the current day from the RTC module. */
        uint8_t getActualDay();

        /**
         * @brief Retrieves the current day of the week from the RTC module.
         * @return Day of the week (0 = Sunday, 1 = Monday, etc.).
         */
        uint8_t getActualDayWeek();

        /** @brief Retrieves the current hour from the RTC module. */
        uint8_t getActualHour();

        /** @brief Retrieves the current minute from the RTC module. */
        uint8_t getActualMinute();

        /** @brief Retrieves the current second from the RTC module. */
        uint8_t getActualSecond();

        /**
         * @brief Converts a DateTime object to a tm struct.
         * @param datetime Reference to the DateTime object.
         * @return Struct tm representing the datetime.
         */
        struct tm getTmDatetime(DateTime &datetime);

        /**
         * @brief Retrieves the current timestamp as a formatted string.
         * @return String representation of the current timestamp.
         */
        String getActualTimestamp();

    private:
        NTPClient ntpClient; /**< NTP client for time synchronization. */
        TimeSpan timespanDatetime; /**< Timespan for next scheduled update. */
        DateTime nextDatetime; /**< Stores the next update time. */
        RTC_DS3231 rtc; /**< RTC module instance. */
        TimeSpan timespanDatetimeRTC; /**< RTC-based timespan for updates. */
        DateTime nextDatetimeRTC; /**< Next RTC-based update time. */
        uint8_t totalMinuteUpdate; /**< Update interval in minutes. */
        uint8_t timeoutCheckRtcDay; /**< Timeout for RTC daily checks. */
        uint16_t timeoutCheckNtpMilliseconds; /**< Timeout for NTP sync. */

        /**
         * @brief Checks if the current RTC datetime exceeds the next scheduled update.
         * @return True if the update interval has elapsed, false otherwise.
         */
        bool checkDatetimeRTC();

        /**
         * @brief Configures the next RTC-based scheduled datetime.
         */
        void configNextDatetimeRTC();

        /**
         * @brief Updates the RTC module with the latest time.
         */
        void updateDatetimeRTC();
};

#endif // DATETIMEINTERVAL_H