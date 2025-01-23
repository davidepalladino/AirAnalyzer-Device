#include "ServerSocketJSON.h"

ServerSocketJSON::ServerSocketJSON(uint16_t port) {
    this->server = nullptr;
    this->port = port;
}

bool ServerSocketJSON::begin() {
    if (!isConnected()) {
        server = new WiFiServer(port);
        server->begin();

        return true;
    }

    return false;
}

void ServerSocketJSON::end() {
    if (isAttached()) {
        client.stop();
    }

    server->stop();
}

bool ServerSocketJSON::attachClient() {
    if (server != nullptr && isConnected() && !isAttached()) {
        client = server->available();

        if (client) {
            Serial.println("Client connected.");
            Serial.println("\tRemote IP: " + client.remoteIP().toString());
            Serial.println("\tRemote Port: " + String(client.remotePort()));
            client.setTimeout(1000);

            return true;
        }
    }

    return false;
}

bool ServerSocketJSON::detachClient() {
    if (isAttached()) {
        client.stop();

        Serial.println("Client disconnected.");
        return true;
    }

    return false;
}

bool ServerSocketJSON::isConnected() {
    if (WiFi.status() == WL_CONNECTED && server != nullptr && server->status() == LISTEN) {
        return true;
    }

    return false;
}

bool ServerSocketJSON::isAttached() {
    if (client) {
        return true;
    }

    return false;
}

StaticJsonDocument<512> ServerSocketJSON::listen() {
    StaticJsonDocument<512> jsonDocumentRequest;

    if (server != nullptr && isConnected() && isAttached()) {
        if (client.connected() && client.available() > 0) {
            jsonRequestSerialized = client.readString();

            if (!jsonRequestSerialized.isEmpty()) {
                /* Deserializing the JSON to get the request code. */
                deserializeJson(jsonDocumentRequest, jsonRequestSerialized);

                client.flush();

            }
        }
    }

    return jsonDocumentRequest;
}

bool ServerSocketJSON::speak(const String &message) {
    if (server != nullptr && isConnected() && isAttached()) {
        client.println(message);
        client.flush();

        return true;
    }
    return false;
}

String ServerSocketJSON::getJsonRequestSerialized() {
    return this->jsonRequestSerialized;
}