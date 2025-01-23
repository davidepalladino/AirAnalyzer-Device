#include "ServerSocketJSON.h"

ServerSocketJSON::ServerSocketJSON() {
    this->server = nullptr;
}

bool ServerSocketJSON::begin() {
    if (!isConnected()) {
        server = new WiFiServer(SERVER_SOCKET_PORT);
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

uint8_t ServerSocketJSON::listen() {
    if (server != nullptr && isConnected() && isAttached()) {
        if (client.connected() && client.available() > 0) {
            jsonRequestSerialized = client.readString();

            if (!jsonRequestSerialized.isEmpty()) {
                /* Deserializing the JSON to get the request code. */
                StaticJsonDocument<SERVER_SOCKET_SIZE_JSON> jsonDocumentRequest;
                deserializeJson(jsonDocumentRequest, jsonRequestSerialized);

                client.flush();

                return jsonDocumentRequest[SERVER_SOCKET_FIELD_REQUEST_CODE];
            }
        }
    }

    return 0;
}

bool ServerSocketJSON::speak(String &message) {
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