/**
 * @file FirmwareUpdateOTA.h
 * @brief Provides functionality to update firmware over-the-air (OTA) at specified intervals.
 *
 * This library facilitates OTA firmware updates by connecting to a specified server and checking for new versions.
 *
 * Copyright (c) 2025 Davide Palladino.
 * All rights reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 3.0.0
 * @date 29th January 2025
 */

#ifndef FIRMWAREUPDATEOTA_H
    #define FIRMWAREUPDATEOTA_H

    #include <Arduino.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266httpUpdate.h>

    #include "FirmwareUpdateOTAConsts.h"

    /**
     * @class FirmwareUpdateOTA
     * @brief Handles over-the-air (OTA) firmware updates.
     *
     * This class enables an ESP8266-based system to connect to an update server,
     * check for new firmware versions, and download updates if available.
     */
    class FirmwareUpdateOTA {
        public:
            /**
             * @brief Constructs a FirmwareUpdateOTA object.
             *

             */
            FirmwareUpdateOTA();

            /**
             * @brief Constructs a FirmwareUpdateOTA object.
             *
             * Initializes the class but does not connect to the update server.
             * Use `begin()` to set up the connection details before checking for updates.
             *
             * @param address The server address hosting the firmware.
             * @param port The server port to connect to.
             * @param uriGetLatest The URI path to check for updates.
             */
            void begin(const String &address, uint16_t port);

            /**
             * @brief Checks for a new firmware version on the server.
             *
             * This method sends a request to the update server, compares the current
             * firmware version with the latest available version, and determines if an update is required.
             *
             * @param version The current firmware version as a string.
             * @return True if an update is available, false otherwise.
             */
            bool check(const String &version);

        private:
            WiFiClient wifiClient;      /**< Client for handling HTTP connections to the update server. */
            String serverAddress;       /**< The address of the update server (e.g., "http://example.com"). */
            uint16_t serverPort;        /**< The port used to connect to the update server (default: 80). */
    };
#endif