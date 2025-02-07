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
 * @version 5.0.0
 * @date 29th January 2025
 */

#ifndef DATETIMEINTERVAL_H
    #define DATETIMEINTERVAL_H

    #include <Arduino.h>
    #include <ESP8266WiFi.h>
    #include <NTPClient.h>
    #include <RTClib.h>
    #include <Wire.h>

    #include <DateTimeIntervalConsts.h>

    /**
    * @class DatetimeInterval
    * @brief Manages time intervals using an RTC module and NTP synchronization.
    *
    * This class provides functionality to synchronize time using an NTP server and an RTC module,
    * allowing periodic updates based on configurable time intervals.
    */
    class DatetimeInterval {
        public:
            /**
             * @brief Constructs a DatetimeInterval object with NTP client settings.
             * @param ntpClient Reference to the NTPClient object for time synchronization.
             */
            explicit DatetimeInterval(NTPClient ntpClient);

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

            /**
             * @brief Retrieves the current year from the RTC module.
             * @return The current year as a 4-digit integer.
             */
            uint16_t getActualYear();

            /**
             * @brief Retrieves the current month from the RTC module.
             * @return The current month (1-12).
             */
            uint8_t getActualMonth();

            /**
             * @brief Retrieves the current day from the RTC module.
             * @return The current day (1-31).
             */
            uint8_t getActualDay();

            /**
             * @brief Retrieves the current day of the week from the RTC module.
             * @return Day of the week (0 = Sunday, 1 = Monday, ..., 6 = Saturday).
             */
            uint8_t getActualDayWeek();

            /**
             * @brief Retrieves the current hour from the RTC module.
             * @return The current hour (0-23).
             */
            uint8_t getActualHour();

            /**
             * @brief Retrieves the current minute from the RTC module.
             * @return The current minute (0-59).
             */
            uint8_t getActualMinute();

            /**
             * @brief Retrieves the current second from the RTC module.
             * @return The current second (0-59).
             */
            uint8_t getActualSecond();

            /**
             * @brief Converts a DateTime object to a tm struct.
             * @param datetime Reference to the DateTime object.
             * @return Struct tm representing the datetime.
             */
            struct tm getTmDatetime(const DateTime &datetime);

            /**
             * @brief Retrieves the current timestamp as a formatted string.
             * @return A string representing the current timestamp in format "YYYY-MM-DD HH:MM:SS".
             */
            String getActualTimestamp();

        private:
            NTPClient ntpClient;             /**< NTP client for time synchronization. */
            TimeSpan timespanDatetime;       /**< Timespan for next scheduled update. */
            DateTime nextDatetime;           /**< Stores the next scheduled update time. */
            RTC_DS3231 rtc;                  /**< RTC module instance. */
            TimeSpan timespanDatetimeRTC;    /**< RTC-based timespan for updates. */
            DateTime nextDatetimeRTC;        /**< Next RTC-based update time. */

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
             * @brief Updates the RTC module with the latest synchronized time.
             */
            void updateDatetimeRTC();
    };

#endif // DATETIMEINTERVAL_H