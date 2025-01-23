/**
 * @brief This library allows to update of firmware on certain time.
 * Copyright (c) 2022 Davide Palladino.
 * All right reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 1.0.2
 * @date 23rd January 2025
 *
 */

#ifndef FIRMWAREUPDATEOTA_H
#define FIRMWAREUPDATEOTA_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

/**
 * @class FirmwareUpdateOTA
 * @brief Handles over-the-air firmware updates.
 *
 * This class allows an ESP8266-based system to check for and download firmware updates
 * from a specified server.
 */
class FirmwareUpdateOTA {
    public:
        /**
         * @brief Constructs a FirmwareUpdateOTA object.
         *
         * @param address The server address hosting the firmware.
         * @param port The server port to connect to.
         */
        FirmwareUpdateOTA(const String& address, uint16_t port);

        /**
         * @brief Initializes the connection settings for the update server.
         *
         * This method should be called before checking for updates.
         */
        void begin();

        /**
         * @brief Checks for a new firmware version on the server.
         *
         * Compares the current firmware version with the server version and determines if an update is available.
         *
         * @param version The current firmware version.
         * @param uri The URI path to check for updates.
         * @return True if an update is available, false otherwise.
         */
        bool check(const String &version, const String &uri);

    private:
        WiFiClient wifiClient; /**< Client for handling HTTP connections. */
        String address; /**< Server address for firmware updates. */
        uint16_t port; /**< Port number for server communication. */
};
#endif