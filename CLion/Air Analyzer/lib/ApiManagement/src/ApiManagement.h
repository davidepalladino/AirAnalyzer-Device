/**
 * @file ApiManagement.h
 * @brief Provides functionality to manage API REST interactions for the Air Analyzer.
 *
 * This library allows managing API REST interactions by acting as an observer,
 * updating its state when notified by the subject class.
 *
 * Copyright (c) 2025 Davide Palladino.
 * All rights reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 4.0.0
 * @date 25th January 2025
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

    #include <ApiManagementConsts.h>

    class Sensor;

    /**
     * @class ApiManagement
     * @brief Manages API interactions for the Air Analyzer system.
     *
     * This class connects to a REST API server, performing operations such as user login,
     * updating room status, and sending measurement data.
     */
    class ApiManagement : private AbstractObserver {
        friend class AbstractSubject;

        public:
            /**
            * @brief Constructs an ApiManagement object and sets the subject class.
            * @param sensor Subject object that will notify.
            * @param datetime Object to check and set the datetime.
            * // FIXME: Is wrong. `sensor` notify any change.
            */
            ApiManagement(Sensor &sensor, DatetimeInterval &datetime);

            /**
             * @brief Initializes the API management system with update intervals.
             * @param address Server address (e.g., "192.168.1.100" or "domain.com").
             * @param port Server port number.
             * @param maxAttempts Number of retry attempts if requests fail (default 0).
             * @param minutesUpdateMeasures Interval for updating measures (default 10 minutes).
             * @warning Call `setCredentials()` first to store the room ID in the API.
             */
            void begin(const String &address, uint16_t port, uint8_t maxAttempts = 0, uint8_t minutesUpdateMeasures = 10);

            /**
             * @brief Sets user credentials for API authentication.
             * @param username User's API username.
             * @param password User's API password.
             */
            void setCredentials(const String &username, const String &password);

            /**
             * @brief Sets the room ID without updating the API.
             * @param roomNumber The room number to be set.
             */
            void setRoomNumber(uint8_t roomNumber);

            /**
             * @brief Gets the current room ID stored in the object.
             * @return The stored room number.
             */
            uint8_t getRoomNumber() const;

            /**
             * @brief Checks if the last update was successful.
             * @return True if the update was successful, false otherwise.
             * // FIXME: Provide right name.
             */
            bool getIsUpdated();

            /**
             * @brief Updates the room with the stored ID and current local IP.
             * @return True if the update was successful, false otherwise.
             */
            bool updateRoom();

        private:
            Sensor &sensor;                                 ///< Reference to the Sensor object.
            DatetimeInterval &datetime;                     ///< Reference to the DatetimeInterval object.
            WiFiClient wifiClient;                          ///< WiFi client for network communication.
            HTTPClient httpClient;                          ///< HTTP client for API requests.
            StaticJsonDocument<512> jsonDocumentLogin;      ///< JSON document for login operations.
            StaticJsonDocument<768> jsonDocumentMeasures;   ///< JSON document for measurement data.
            JsonArray jsonArrayMeasures;                    ///< JSON array for storing measurement data.
            String httpJsonResponse;                        ///< Holds server responses.
            String serverAddress;                           ///< API server address.
            uint16_t serverPort;                            ///< API server port.
            String serverUsername;                          ///< API username.
            String serverPassword;                          ///< API password.
            String serverToken;                             ///< Token received after login.
            String serverTokenType;                         ///< Type of token received (e.g., Bearer).
            uint8_t roomNumber;                             ///< Room number identifier.
            uint8_t maxAttempts;                            ///< Maximum retry attempts.
            bool isUpdated;                                 ///< Indicates whether the last update was successful.

            /**
             * @brief Sends a login request to the server.
             * @return HTTP status code returned by the server.
             */
            int requestLogin();

            /**
             * @brief Sends a request to change the room activation status.
             * @return HTTP status code returned by the server.
             */
            int requestChangeStatusActivationRoom();

            /**
             * @brief Sends a request to update the room's local IP address.
             * @param localIP The current local IP address of the device.
             * @return HTTP status code returned by the server.
             */
            int requestChangeLocalIpRoom(const String &localIP);

            /**
             * @brief Sends measurement data to the server.
             * @param jsonDocumentMeasuresSerialized Serialized JSON string of the measurement data.
             * @return HTTP status code returned by the server.
             */
            int requestSetMeasures(const String &jsonDocumentMeasuresSerialized);

            /**
             * @brief Handles the login process by authenticating with the API.
             * @return HTTP status code indicating success or failure.
             */
            int login();

            /**
             * @brief Adds measurement data to the JSON array.
             * @param timestamp Measurement timestamp in ISO 8601 format.
             * @param temperature Temperature value in Celsius.
             * @param humidity Humidity value in percentage.
             * @return True if the data was successfully added, false otherwise.
             */
            bool addMeasures(const String &timestamp, double temperature, double humidity);

            /**
             * @brief Updates the current status with new values from the sensor.
             *
             * This function is called whenever the observed sensor provides new data.
             * // FIXME: Is wrong. update receives values.
             */
            void update();
    };

#endif // APIMANAGEMENT_H
