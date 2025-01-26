/**
 * @file ServerSocketJSON.h
 * @brief This library allows the device to function as a server socket, exchanging JSON messages.
 *
 * The structure of the JSON message is expected to be:
 * {
 *  "Request code": <number between 0 and 255>,
 *  "message": { <JSON nested> }
 * }
 *
 * Copyright (c) 2025 Davide Palladino.
 * All rights reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 2.0.1
 * @date 25th January 2025
 *
 * @include ArduinoJson v6.18.2.
 */

#ifndef SERVERSOCKETJSON_H
    #define SERVERSOCKETJSON_H

    #include <Arduino.h>
    #include <ESP8266WiFi.h>

    #include "ServerSocketJSONConsts.h"

    #ifndef ARDUINO_JSON
        #include "ArduinoJson.h"
    #endif

    /**
     * @class ServerSocketJSON
     * @brief A class that represents a server socket for handling JSON messages with a client.
     *
     * This class enables the device to act as a server, receiving and sending JSON messages via Wi-Fi.
     * It provides methods for managing clients, checking server/client status, and sending/receiving JSON messages.
     */
    class ServerSocketJSON {
        public:
            /**
             * @brief Constructor to initialize the server socket.
             */
            ServerSocketJSON();

            /**
             * @brief Initializes the server socket and begins listening for clients.
             *
             * This method starts the server socket on the specified port and ensures Wi-Fi is connected.
             *
             * @param port The port to listen for incoming connections.
             * @return True if the server was successfully started, false if Wi-Fi is not connected.
             */
            bool begin(uint16_t port);

            /**
             * @brief Closes the server and any active client connection.
             */
            void end();

            /**
             * @brief Attaches a client to the server.
             *
             * If no client is currently attached, this method establishes a connection with an available client.
             *
             * @return True if a client was successfully attached, false otherwise.
             */
            bool attachClient();

            /**
             * @brief Detaches the currently connected client.
             *
             * Disconnects the active client, freeing up the server to accept new connections.
             *
             * @return True if a client was successfully detached, false if no client is attached.
             */
            bool detachClient();

            /**
             * @brief Checks if the server is actively running.
             *
             * Determines whether the server is active and ready to accept client connections.
             *
             * @return True if the server is running, false otherwise.
             */
            bool isConnected();

            /**
             * @brief Checks if a client is currently connected.
             *
             * @return True if a client is attached, false otherwise.
             */
            bool isAttached();

            /**
             * @brief Listens for an incoming JSON message from the client.
             *
             * This method blocks until a message is received, then deserializes it into a JSON object.
             *
             * @return A status code indicating the message reception outcome.
             */
            uint8_t listen();

            /**
             * @brief Sends a JSON message to the connected client.
             *
             * Serializes and transmits a JSON-formatted message to the client.
             *
             * @param message The JSON message to be sent in string format.
             * @return True if the message was successfully sent, false otherwise.
             */
            bool speak(const String &message);

            /**
             * @brief Retrieves the last received JSON message in serialized form.
             *
             * This method provides access to the most recently received JSON message.
             *
             * @return A string containing the raw JSON message received from the client.
             */
            String getJsonRequestSerialized();

        private:
            WiFiServer *server;                 ///< Pointer to the WiFiServer instance for handling client connections.
            WiFiClient client;                  ///< WiFiClient instance to interact with the connected client.
            String jsonRequestSerialized;       ///< Holds the last received serialized JSON request.
    };

#endif
