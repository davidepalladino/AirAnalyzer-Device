 /**
  * @brief This library allows to manage the apiManagement with API REST, for the Air Analyzer purposes.
  * The class is an observer where updates own status when is notified by the subject class.
  * Copyright (c) 2022 Davide Palladino.
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact davidepalladino@hotmail.com
  * @website https://davidepalladino.github.io/
  * @version 3.0.0
  * @date 30th September, 2022
  * 
  */

#ifndef APIMANAGEMENT_H
    #define APIMANAGEMENT_H

    #include <map>

    #include <Arduino.h>

    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>

    #include <ArduinoJson.h>

    #include <AbstractObserver.h>
    #include <DatetimeInterval.h>
    #include <Sensor.h>

    #include "ApiRoute.h"

    class Sensor;

    class ApiManagement : private AbstractObserver {
        friend class AbstractSubject;

        public:
            /**
             * @brief This constructor creates the object setting subject class.
             * @param sensor Subject object that will notify.
             * @param datetime Object to check and set the datetime.
             */
            ApiManagement(Sensor &sensor, DatetimeInterval &datetime);

            /**
             * @brief This method provides to set Datetime object, the JSON array and the information about the
             *  connection to the server, with the timeout (in minutes) for every update.
             * @param address URL for the connection to the server.
             * @param port Port for the connection to the server.
             * @param nAttempts Number of attempts for login. Default value is "0".
             * @param timeoutMinutes Minutes for every update. Default value is "10".
             * @warning Previously will have to be called the "setCredentials()" method, because "begin()" stores the room ID into apiManagement; so it needs the credentials.
             */
            void begin(const String &address, uint16_t port, uint8_t nAttempts = 0, uint8_t timeoutMinutes = 10);

            /**
             * This method provides to set the credentials of user.
             * @param username Username about the user for managing the own apiManagement.
             * @param password Password about the user for managing the own apiManagement.
             */
            void setCredentials(const String &username, const String &password);

            /**
             * @brief This method sets only the room ID. The apiManagement will be not updated.
             * @param roomNumber Number of room.
             */
            void setRoomNumber(uint8_t roomNumber);

            /**
             * @brief This method gets the room ID.
             * @return Number of room.
             */
            uint8_t getRoomNumber() const;

            /**
             * @brief This method gets the status indicator about the error on update.
             * @return Value "true" if the update has been successful; else, value "false".
             */
            bool getIsUpdated();

            /**
             * @brief This method provides to update the room with the ID set (with "setRoom()") and the current local IP.
             * @return Value "true" if the update has been successful; else, value "false".
             */
            bool updateRoom();

        private:
            Sensor &sensor;
            DatetimeInterval &datetime;
            WiFiClient wifiClient;
            HTTPClient httpClient;
            StaticJsonDocument<512> jsonDocumentLogin;
            StaticJsonDocument<768> jsonDocumentMeasures;
            JsonArray jsonArrayMeasures;
            String httpJsonResponse;
            String serverAddress;
            uint16_t serverPort;
            String serverUsername;
            String serverPassword;
            String serverToken;
            String serverTokenType;
            uint8_t roomNumber;
            uint8_t nAttempts;
            bool isUpdated;

            /**
             * This method provides to execute a POST request to a specicif URI.
             * @param uri URI for the request.
             * @param headers Header in format Key => Value.
             * @param body Body in format Key => Value.
             * @return HTTP Status Code.
             */
            int requestPost(String uri, std::map<String, String> headers, std::map<String, String> body);

            /**
             * This method provides to execute a POST request to a specicif URI.
             * @param uri URI for the request.
             * @param headers Header in format Key => Value.
             * @param body Body in format String.
             * @return HTTP Status Code.
             */
            int requestPost(String uri, std::map<String, String> headers, String body);

            /**
             * This method provides to execute a PATCH request to a specicif URI.
             * @param uri URI for the request.
             * @param headers Header in format Key => Value.
             * @param body Body in format Key => Value.
             * @return HTTP Status Code.
             */
            int requestPatch(String uri, std::map<String, String> headers, std::map<String, String> body);

            /**
             * @brief This method provides to login the user.
             * @return Value "true" if there login has been successful; else, value "false".
             */
            uint16_t login();

            /**
             * @brief This method provides to login the user.
             * @return Value "true" if there login has been successful; else, value "false".
             */
            bool addMeasures(const String &timestamp, double temperature, double humidity);

            /**
             * @brief This method insert the actual values into apiManagement.
             */
            void update();
    };
#endif