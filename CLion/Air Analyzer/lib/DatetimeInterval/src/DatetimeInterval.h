 /**
  * @brief This library allows to manage an interval of time between the actual (stored on RTC module and get with NTP object)
  * and the next set. 
  * Copyright (c) 2022 Davide Palladino.
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact davidepalladino@hotmail.com
  * @website https://davidepalladino.github.io/
  * @version 3.0.3
  * @date 30th September, 2022
  * 
  */

#ifndef DATETIMEINTERVAL_H
    #define DATETIMEINTERVAL_H

    #include <Arduino.h>

    #include <ESP8266WiFi.h>

    #include <NTPClient.h>
    #include <RTClib.h>

    #include <ctime>

    #define TIMEOUT_CHECK_RTC_DAY 14
    #define TIMEOUT_CHECK_NTP_MILLISECONDS 5000                                            // Timeout to check the datetime from NTP.

    class DatetimeInterval {
        public:
            /** 
             * @brief This constructor creates this object setting the NTPClient object.
             */
            explicit DatetimeInterval(NTPClient ntpClient);

            /** 
             * @brief This method initializes the RTC object and sets the next datetime.
             * @param totalMinuteUpdate Total minutes for every update.
             * @warning The max minutes will be 240. Every value greater than this will be considered as such.
             */
            void begin(uint8_t totalMinuteUpdate);

            /** 
             * @brief This method indicates if the actual datetime is greater than next.
             * @return Boolean value "true" to indicate if there is a variation; else value "false".
             */
            bool checkDatetime();

            /** 
             * @brief This method configs the next datetime, based on "timespanDatetime".
             */
            void configNextDatetime();

            /** 
             * @brief This method gets the year from the RCT object.
             * @return Actual year.
             */
            uint16_t getActualYear();

            /** 
             * @brief This method gets the month from the RCT object.
             * @return Actual month.
             */
            uint8_t getActualMonth();

            /** 
             * @brief This method gets the day from the RCT object.
             * @return Actual day.
             */
            uint8_t getActualDay();

            /** 
             * @brief This method gets the day of week from the RCT object.
             * @return Actual day of week. Value "0" is about to sunday; "1" for monday.
             */
            uint8_t getActualDayWeek();

            /** 
             * @brief TThis method gets the hour from the RCT object.
             * @return Actual hour.
             */
            uint8_t getActualHour();

            /** 
             * @brief This method gets the minute from the RCT object.
             * @return Actual minute.
             */
            uint8_t getActualMinute();

            /** 
             * @brief This method gets the second from the RCT object.
             * @return Actual second .
             */
            uint8_t getActualSecond();

            /**
             * @brief This method gets a struct "tm" from a Datetime object.
             * @return Struct "tm" with the datetime.
             */
            struct tm getTmDatetime(DateTime &datetime);

            /**
             * @brief This method gets the timestamp of actual datetime.
             * @return String with timestamp.
             */
            String getActualTimestamp();

        private:
            NTPClient ntpClient;
            TimeSpan timespanDatetime;
            DateTime nextDatetime;
            RTC_DS3231 rtc;
            TimeSpan timespanDatetimeRTC;
            DateTime nextDatetimeRTC;

            /**
             * @brief This method indicates, for RTC object, if the actual datetime is greater than next.
             * @return Boolean value "true" to indicate if there is a variation; else value "false".
             */
            bool checkDatetimeRTC();

            /**
             * @brief This method configs the next datetime for RTC object, based on "timespanDatetimeRTC".
             */
            void configNextDatetimeRTC();

            /**
             * @brief This method updates the RTC module.
             */
            void updateDatetimeRTC();
    };
#endif