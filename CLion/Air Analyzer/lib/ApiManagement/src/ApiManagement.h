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
 * @date 23rd January 2025
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
         */
        ApiManagement(Sensor &sensor, DatetimeInterval &datetime);

        /**
         * @brief Initializes the API management system with update intervals.
         * @warning Call "setCredentials()" first to store the room ID in the API.
         */
        void begin();

        /**
         * @brief Sets user credentials.
         * @param username User's username for API access.
         * @param password User's password for API access.
         */
        void setCredentials(const String &username, const String &password);

        /**
         * @brief Sets the room ID without updating the API.
         * @param roomNumber The room number.
         */
        void setRoomNumber(uint8_t roomNumber);

        /**
         * @brief Gets the current room ID.
         * @return The room number.
         */
        uint8_t getRoomNumber() const;

        /**
         * @brief Checks if the last update was successful.
         * @return True if the update was successful, false otherwise.
         */
        bool getIsUpdated();

        /**
         * @brief Updates the room with the stored ID and current local IP.
         * @return True if the update was successful, false otherwise.
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
        bool isUpdated;

        /**
         * @brief Sends a login request to the server.
         * @return HTTP status code.
         */
        int requestLogin();

        /**
         * @brief Sends a request to change the room activation status.
         * @return HTTP status code.
         */
        int requestChangeStatusActivationRoom();

        /**
         * @brief Sends a request to update the local IP of the room.
         * @param localIP The current local IP address.
         * @return HTTP status code.
         */
        int requestChangeLocalIpRoom(const String &localIP);

        /**
         * @brief Sends measurement data to the server.
         * @param jsonDocumentMeasuresSerialized Serialized JSON of measurements.
         * @return HTTP status code.
         */
        int requestSetMeasures(const String &jsonDocumentMeasuresSerialized);

        /**
         * @brief Handles the login process.
         * @return True if login was successful, false otherwise.
         */
        int login();

        /**
         * @brief Adds measurement data.
         * @param timestamp Measurement timestamp.
         * @param temperature Temperature value.
         * @param humidity Humidity value.
         * @return True if the data was successfully added, false otherwise.
         */
        bool addMeasures(const String &timestamp, double temperature, double humidity);

        /**
         * @brief Updates the current status with new values.
         */
        void update();
};

#endif // APIMANAGEMENT_H
