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

    const String FIRMWARE_UPDATE_OTA_BASE_URL = "http://airanalyzer.shadowmoses.ovh";
    constexpr uint16_t FIRMWARE_UPDATE_OTA_BASE_PORT = 80;
    const String FIRMWARE_UPDATE_OTA_URI_GET_LATEST = "api/firmware/getLatest";

    class FirmwareUpdateOTA {
    public:
        /**
         * @brief This constructor creates the object only.
         */
        FirmwareUpdateOTA();

        /**
         * @brief This method provides to set the information about the connection to the server.
         */
        void begin();

        /**
         * @brief This method provides to check if there is a new version of firmware.
         * @param version Actual version of firmware.
         * @return
         */
        bool check(const String &version);

    private:
        WiFiClient wifiClient;
        String serverAddress;
        uint16_t serverPort;
        String serverUri;
    };
#endif