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
 * @date 23rd January 2025
 *
 * @include ArduinoJson v6.18.2.
 */

#ifndef SERVERSOCKETJSON_H
#define SERVERSOCKETJSON_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#ifndef ARDUINO_JSON
    #include "ArduinoJson.h"
#endif

/**
 * @class ServerSocketJSON
 * @brief A class that represents a server socket for handling JSON messages with a client.
 *
 * This class allows the device to act as a server, receiving and sending JSON messages to a client via Wi-Fi.
 * It provides methods for attaching/detaching clients, checking server/client status, and communicating using JSON format.
 */
class ServerSocketJSON {
    public:
        /**
         * @brief Constructor to initialize the server socket with a specified port.
         * @param port The port to open for the server.
         */
        ServerSocketJSON(uint16_t port);

        /**
         * @brief Initializes the server socket and begins listening for clients.
         *
         * This method opens the server socket on the specified port and connects to Wi-Fi if not already connected.
         * @return True if the server was successfully opened, false if there was an issue (e.g., Wi-Fi not connected).
         */
        bool begin();

        /**
         * @brief Closes the server and any client connections.
         */
        void end();

        /**
         * @brief Attaches a client to the server.
         *
         * If no client is currently attached, this method will establish a connection with the first available client.
         * @return True if a client was successfully attached, false if the server or Wi-Fi is not connected, or if a client is already attached.
         */
        bool attachClient();

        /**
         * @brief Detaches the currently attached client.
         *
         * If a client is attached, it will be disconnected, freeing up the server for other clients.
         * @return True if a client was successfully detached, false if no client is currently attached.
         */
        bool detachClient();

        /**
         * @brief Checks whether the server is connected and ready to accept connections.
         * @return True if the server is connected, false if Wi-Fi or the server is not connected.
         */
        bool isConnected();

        /**
         * @brief Checks whether a client is currently connected.
         * @return True if a client is attached, false otherwise.
         */
        bool isAttached();

        /**
         * @brief Listens for a JSON message from the attached client.
         *
         * This method listens for incoming messages from the client, deserializing the JSON structure.
         * @return A StaticJsonDocument containing the JSON message received.
         */
        StaticJsonDocument<512> listen();

        /**
         * @brief Sends a JSON message to the attached client.
         *
         * This method sends a JSON-serialized message to the client. The message must follow the expected JSON structure.
         * @param message The JSON message to send to the client.
         * @return True if the message was successfully sent, false if the server or Wi-Fi connection is not active.
         */
        bool speak(const String &message);

        /**
         * @brief Retrieves the last serialized JSON message received from the client.
         *
         * This method returns the last received message in its raw serialized form as a string.
         * @return A string containing the raw JSON message received from the client.
         */
        String getJsonRequestSerialized();

    private:
        WiFiServer *server;           ///< Pointer to the WiFiServer instance.
        WiFiClient client;            ///< WiFiClient instance to interact with the client.
        String jsonRequestSerialized; ///< Holds the serialized JSON request.
        uint16_t port;                ///< Port number for the server socket.
};

#endif
