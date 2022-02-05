/**
 * @brief This library allows to update of firmware on certain time.
 * Copyright (c) 2020 Davide Palladino.
 * All right reserved.
 *
 * @author Davide Palladino
 * @contact me@davidepalladino.com
 * @website www.davidepalladino.com
 * @version 1.0.0
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
        WiFiClient wiFiClient;
        String serverAddress;
        uint16_t serverPort;
        String serverUri;
    };
#endif