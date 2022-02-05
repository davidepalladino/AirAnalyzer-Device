/**
 * @brief This library allows the device to work like server socket, exchanging JSON messages.
 * The structure of JSON message will have to be:
 * {
 *  "Request code": <number between 0 and 255>,
 *  "Message": { <JSON nested> }
 * }
 *
 * Copyright (c) 2020 Davide Palladino.
 * All right reserved.
 *
 * @author Davide Palladino
 * @contact me@davidepalladino.com
 * @website www.davidepalladino.com
 * @version 1.0.0
 * @date 8th August, 2021
 *
 * @include ArduinoJson v6.18.2.
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

#ifndef SERVERSOCKETJSON_H
    #define SERVERSOCKETJSON_H

    #include <Arduino.h>
    #include <ESP8266WiFi.h>

    #ifndef ARDUINO_JSON
        #include "ArduinoJson.h"
    #endif

    const uint16_t SIZE_JSON_DOCUMENT = 512;

    class ServerSocketJSON {
        public:
            /**
             * @brief This constructor provides to initialize the object.
             */
            ServerSocketJSON();

            /**
             * @brief This method opens the server socket on a specific port.
             * @param port Number of port where the clients will be able to connect.
             * @return Value "true" if the server is opened; else, value "false". In this case the reason might be a not
             *  connection of WiFi, or the server already connected.
             */
            bool begin(uint16_t port);

            /**
             * @brief This method closes the server and client connections.
             */
            void end();

            /**
             * @brief This method provides to attach the client, if someone is not already attached.
             * @return Value "true" if the attaching has been successful; else value "false" to indicate that the attaching
             *  has been not successful for several reason like the server and/or WiFi not connected, or there is another client attached.
             */
            bool attachClient();

            /**
             * @brief This method provides to detach the client connected.
             * @return Value "true" if the detaching has been successful; else value "false" to indicate that the detaching
             *  has been not successful because there is not client attached.
             */

            bool detachClient();

            /**
             * @brief This method provides to check the server connection.
             * @return Value "true" if the server is connected; else value "false" to indicate that the server is not connected
             *  because there is not connection of WiFi.
             */
            bool isConnected();

            /**
             * @brief This method provides to check if there is a client connected.
             * @return Value "true" if there is a client connected; else, value "false".
             */
            bool isAttached();

            /**
             * @brief This method provides to listen a client JSON message.
             * @return Value of "Request code" contained on JSON message.
             */
            uint8_t listen();

            /**
             * @brief This method provides to send a specific JSON message to client.
             * @param message JSON Message to send to client.
             * @return Value "true" if the JSON message has been sent; else, value "false" to indicate a server and/or WiFi not connected.
             */
            bool speak(String &message);

            /**
             * @brief This method provides to get the last JSON message from client, to deserialize.
             * @return Field "Message" of JSON message to deserialize.
             */
            String getJsonRequestSerialized();

        private:
            WiFiServer *server;
            WiFiClient client;
            String jsonRequestSerialized;
    };
#endif