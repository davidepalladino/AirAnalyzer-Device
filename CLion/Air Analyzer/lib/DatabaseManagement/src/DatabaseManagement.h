 /**
  * @brief This library allows to manage the database with API REST, for the Air Analyzer purposes.
  * The class is an observer where updates own status when is notified by the subject class.
  * Copyright (c) 2020 Davide Palladino.
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact me@davidepalladino.com
  * @website www.davidepalladino.com
  * @version 2.0.2
  * @date 10th August, 2021
  *
  * This library is free software; you can redistribute it and/or
  *  modify it under the terms of the GNU General Public
  *  License as published by the Free Software Foundation; either
  *  version 3.0 of the License, or (at your option) any later version
  * 
  * This library is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  *  GNU Lesser General Public License for more details.
  * 
  */

#ifndef DATABASEMANAGEMENT_H
    #define DATABASEMANAGEMENT_H

    #include <Arduino.h>

    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>

    #include <ArduinoJson.h>

    #include <AbstractObserver.h>
    #include <DatetimeInterval.h>
    #include <Sensor.h>

    #include "API.h"

    class Sensor;

    class DatabaseManagement : private AbstractObserver {
        friend class AbstractSubject;

        public:
            /**
             * @brief This constructor creates the object setting subject class.
             * @param sensor Subject object that will notify.
             * @param datetime Object to check and set the datetime.
             */
            DatabaseManagement(Sensor &sensor, DatetimeInterval &datetime);

            /**
             * @brief This method provides to set Datetime object, the JSON array and the information about the
             *  connection to the server, with the timeout (in minutes) for every update.
             * @param address URL for the connection to the server.
             * @param port Port for the connection to the server.
             * @param nAttempts Number of attempts for login. Default value is "0".
             * @param timeoutMinutes Minutes for every update. Default value is "10".
             * @warning Previously will have to be called the "setCredentials()" method, because "begin()" stores the room ID into database; so it needs the credentials.
             */
            void begin(const String &address, uint16_t port, uint8_t nAttempts = 0, uint8_t timeoutMinutes = 10);

            /**
             * This method provides to set the credentials of user.
             * @param username Username about the user for managing the own database.
             * @param password Password about the user for managing the own database.
             */
            void setCredentials(const String &username, const String &password);

            /**
             * @brief This method sets only the room ID. The database will be not updated.
             * @param roomID Number of room.
             */
            void setRoomID(uint8_t roomID);

            /**
             * @brief This method gets the room ID.
             * @return Number of room.
             */
            uint8_t getRoomID() const;

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
            StaticJsonDocument<1024> jsonDocumentLogin;
            StaticJsonDocument<640> jsonDocumentMeasures;
            JsonArray jsonArrayMeasures;
            String httpJsonResponse;
            String serverAddress;
            uint16_t serverPort;
            String serverUsername;
            String serverPassword;
            String serverToken;
            String serverTokenType;
            uint8_t roomID;
            uint8_t nAttempts;
            bool isUpdated;

            /**
             * @brief This method provides to connect to the server for doing the login.
             * @return HTTP status code.
             */
            uint16_t requestPostLogin();

            /**
             * @brief This method provides to connect to the server for adding the room.
             * @return HTTP status code.
             */
            uint16_t requestPostAddRoom();

            /**
             * @brief This method provides to connect to the server for updating the actual local IP.
             * @return HTTP status code.
             */
            uint16_t requestPostUpdateLocalIPRoom(const String &localIP);

            /**
             * @brief This method provides to connect to the server for adding the measures in JSON format.
             * @return HTTP status code.
             */
            uint16_t requestPutMeasures(const String &jsonDocumentMeasuresSerialized);

            /**
             * @brief This method provides to login the user.
             * @return Value "true" if there login has been successful; else, value "false".
             */
            bool login();

            /**
             * @brief This method provides to login the user.
             * @return Value "true" if there login has been successful; else, value "false".
             */
            bool addMeasures(const String &timestamp, double temperature, double humidity);

            /**
             * @brief This method insert the actual values into database.
             */
            void update();
    };
#endif