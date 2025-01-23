/**
 * @file FirmwareUpdateOTA.h
 * @brief Provides functionality to update firmware over-the-air (OTA) at specified intervals.
 *
 * This library facilitates OTA firmware updates by connecting to a specified server and checking for new versions.
 *
 * Copyright (c) 2022 Davide Palladino.
 * All rights reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 2.0.0
 * @date 23rd January 2025
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
         * @param uriGetLatest The URI path to check for updates.
         */
        FirmwareUpdateOTA(const String& address, uint16_t port, const String &uriGetLatest);

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
         * @return True if an update is available, false otherwise.
         */
        bool check(const String &version);

    private:
        WiFiClient wifiClient; /**< Client for handling HTTP connections. */
        String address; /**< Server address for firmware updates. */
        uint16_t port; /**< Port number for server communication. */
        String uriGetLatest; /**< URI path to check for firmware updates. */
};
#endif