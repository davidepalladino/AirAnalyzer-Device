/**
 * @brief This library allows to update of firmware on certain time.
 * Copyright (c) 2022 Davide Palladino.
 * All right reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 1.0.1
  * @date 30th September, 2022
 *
 */

#ifndef FIRMWAREUPDATEOTA_H
    #define FIRMWAREUPDATEOTA_H

    #include <Arduino.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266httpUpdate.h>

    class FirmwareUpdateOTA {
    public:
        /**
         * @brief This constructor creates the object only.
         */
        FirmwareUpdateOTA();

        /**
         * @brief This method provides to set the information about the connection to the server.
         * @param address URL for the connection to the server.
         * @param port Port for the connection to the server.
         * @param uri URI of the service.
         */
        void begin(const String &address, uint16_t port, const String &uri);

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