#include "ServerSocketJson.h"

ServerSocketJson::ServerSocketJson() {
    this->server = nullptr;
}

bool ServerSocketJson::begin(uint16_t port) {
    if (!isConnected()) {
        server = new WiFiServer(port);
        server->begin();

        return true;
    }

    return false;
}

void ServerSocketJson::end() {
    if (isAttached()) {
        client.stop();
    }

    server->stop();
}

bool ServerSocketJson::attachClient() {
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

bool ServerSocketJson::detachClient() {
    if (isAttached()) {
        client.stop();

        Serial.println("Client disconnected.");
        return true;
    }

    return false;
}

bool ServerSocketJson::isConnected() {
    if (WiFi.status() == WL_CONNECTED && server != nullptr && server->status() == LISTEN) {
        return true;
    }

    return false;
}

bool ServerSocketJson::isAttached() {
    if (client) {
        return true;
    }

    return false;
}

StaticJsonDocument<512> ServerSocketJson::listen() {
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

bool ServerSocketJson::speak(const String &message) {
    if (server != nullptr && isConnected() && isAttached()) {
        client.println(message);
        client.flush();

        return true;
    }
    return false;
}

String ServerSocketJson::getJsonRequestSerialized() {
    return this->jsonRequestSerialized;
}