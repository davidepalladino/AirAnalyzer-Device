#include <DatabaseManagement.h>

DatabaseManagement::DatabaseManagement(Sensor &sensor, DatetimeInterval &datetime) : sensor(sensor), datetime(datetime) {
    this->sensor.addObserver(this);

    this->serverPort = 0;
    this->roomID = 0;
    this->nAttempts = 0;

    this->isUpdated = true;
}

void DatabaseManagement::begin(const String &address, uint16_t port, const String &fingerprint, uint8_t nAttempt, uint8_t timeoutMinutes) {
    Serial.println("\033[1;92m-------------------- [DATABASE] -------------------\033[0m");
    this->datetime.begin(timeoutMinutes > 240 ? 240 : timeoutMinutes);
    this->jsonArrayMeasures = jsonDocumentMeasures.to<JsonArray>();

    this->serverAddress = address;
    this->serverPort = port;
    this->fingerprint = fingerprint;

    this->nAttempts = nAttempt;

    this->wifiClient.setFingerprint(this->fingerprint);

    while (!updateRoom()) { delay(1000); };
}

void DatabaseManagement::setCredentials(const String &serverUsername, const String &serverPassword) {
    this->serverUsername = serverUsername;
    this->serverPassword = serverPassword;
}

void DatabaseManagement::setRoomID(uint8_t roomID) { this->roomID = roomID; }

uint8_t DatabaseManagement::getRoomID() const { return roomID; }

bool DatabaseManagement::getIsUpdated() { return isUpdated; }

bool DatabaseManagement::updateRoom() {
    if (WiFi.status() == WL_CONNECTED) {
        if (login()) {
            if (requestPostAddRoom() == 200 && requestPostUpdateLocalIPRoom(WiFi.localIP().toString()) == 200) {
                isUpdated = true;
            } else {
                isUpdated = false;
            }

            return isUpdated;
        } else {
            isUpdated = false;
            return isUpdated;
        }
    } else if (WiFi.status() == WL_DISCONNECTED) {
        isUpdated = false;
        return isUpdated;
    } else {
        isUpdated = false;
        return isUpdated;
    }
}

uint16_t DatabaseManagement::requestPostLogin() {
//    if (!wifiClient.connect(serverAddress, serverPort)) {
//        Serial.println("Connection failed");
//        delay(1000);
//    } else {
//        Serial.println("Connection OK");
//        delay(1000);
//    }

    httpClient.begin(wifiClient, serverAddress, serverPort, API_ADD_ROOM, true);
    if (httpClient.connected()) {
        httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
        uint16_t responseCode = httpClient.POST("username=" + serverUsername + "&" + "password=" + serverPassword);
        httpJsonResponse = httpClient.getString();
        httpClient.end();

        return responseCode;
    } else {
        return 404;
    }
}

uint16_t DatabaseManagement::requestPostAddRoom() {
    httpClient.begin(wifiClient, serverAddress, serverPort, API_ADD_ROOM, true);
    if (httpClient.connected()) {
        httpClient.addHeader("Authorization", serverTokenType + " " + serverToken);
        httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
        uint16_t responseCode = httpClient.POST("ID=" + String(roomID));
        httpClient.end();

        return responseCode;
    } else {
        return 404;
    }
}

uint16_t DatabaseManagement::requestPostUpdateLocalIPRoom(const String &localIP) {
    httpClient.begin(wifiClient, serverAddress, serverPort, API_UPDATE_LOCAL_IP_ROOM, true);
    if (httpClient.connected()) {
        httpClient.addHeader("Authorization", serverTokenType + " " + serverToken);
        httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
        uint16_t responseCode = httpClient.POST("ID=" + String(roomID) + "&LocalIP=" + localIP);
        httpClient.end();

        return responseCode;
    } else {
        return 404;
    }
}

uint16_t DatabaseManagement::requestPutMeasures(const String &jsonDocumentMeasuresSerialized) {
    httpClient.begin(wifiClient, serverAddress, serverPort, API_SET_MEASURES, true);
    if (httpClient.connected()) {
        httpClient.addHeader("Authorization", serverTokenType + " " + serverToken);
        httpClient.addHeader("Content-Type", "application/json");
        httpClient.addHeader("Accept", "application/json");
        uint16_t responseCode = httpClient.PUT(jsonDocumentMeasuresSerialized);
        httpClient.end();

        return responseCode;
    } else {
        return 404;
    }

}

bool DatabaseManagement::login() {
    uint8_t countAttempts = 1;

    /* Login into the server and taking the token. */
    do {
        yield();

        switch (requestPostLogin()) {
            case 200:
                /* Storing the token for next purposes. */
                jsonDocumentLogin["token"]["token_type"] = true;
                deserializeJson(jsonDocumentLogin, httpJsonResponse);
                serverToken = (String) jsonDocumentLogin["token"];
                serverTokenType = (String) jsonDocumentLogin["token_type"];

                return true;
        }

    } while (countAttempts++ < nAttempts);

    return false;
}

bool DatabaseManagement::addMeasures(const String &timestamp, double temperature, double humidity) {
    /* Creating the JSON body. */
    if (jsonArrayMeasures.size() == 5) {
        jsonArrayMeasures.remove(0);
    }

    JsonObject measures = jsonArrayMeasures.createNestedObject();
    measures["DateAndTime"] = timestamp;
    measures["Room"] = roomID;
    measures["Temperature"] = String(temperature, 2);
    measures["Humidity"] = String(humidity, 2);

    String jsonDocumentMeasuresSerialized;
    serializeJson(jsonArrayMeasures, jsonDocumentMeasuresSerialized);

    /*
     * Check the connection and adding measures if there is connection and the login has been successful,
     *  or reconnecting in case of error.
     */
    if (WiFi.status() == WL_CONNECTED) {
        if (login()) {
            if (isUpdated) {
                requestPostUpdateLocalIPRoom(WiFi.localIP().toString());
            }

            requestPutMeasures(jsonDocumentMeasuresSerialized);

            Serial.println("\033[1;92m---------------- [TRANSACTION JSON] ---------------\033[0m");
            for (auto && jsonArrayMeasure : jsonArrayMeasures) {
                yield();

                JsonObject measure = jsonArrayMeasure.as<JsonObject>();
                Serial.println("\033[1;92mVALUES AT " + (String) measure["DateAndTime"] + "\033[0m");
                Serial.println("\t\033[1;97mTEMPERATURE:   " + String(measure["Temperature"]) + "\033[0m");
                Serial.println("\t\033[1;97mHUMIDITY:      " + String(measure["Humidity"]) + "\033[0m");
            }
            Serial.println("\033[1;92m---------------------------------------------------\033[0m\n");

            jsonArrayMeasures.clear();
            jsonDocumentMeasures.garbageCollect();

            isUpdated = true;
            return isUpdated;
        } else {
            isUpdated = false;
            return isUpdated;
        }
    } else if (WiFi.status() == WL_DISCONNECTED) {
        Serial.println("\033[1;91m[WIFI ERROR FROM DatabaseManagement]\033[0m");

        isUpdated = false;
        return isUpdated;
    } else {
        isUpdated = false;
        return isUpdated;
    }
}

void DatabaseManagement::update() {
    if (datetime.checkDatetime()) {
        Serial.println("\033[1;92m-------------------- [DATABASE] -------------------\033[0m");

        addMeasures(datetime.getActualTimestamp(), sensor.getTemperature(), sensor.getHumidity());
        datetime.configNextDatetime();

        Serial.println("\033[1;96m[FREE HEAP SIZE: " + String(ESP.getFreeHeap()) + "]\033[0m");
    }
}