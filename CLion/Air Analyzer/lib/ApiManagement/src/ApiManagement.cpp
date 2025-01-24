#include <ApiManagement.h>

ApiManagement::ApiManagement(Sensor &sensor, DatetimeInterval &datetime) : sensor(sensor), datetime(datetime) {
    this->sensor.addObserver(this);
}

void ApiManagement::begin(const String &address, uint16_t port, uint8_t maxAttempts, uint8_t minutesUpdateMeasures) {
    Serial.println("\033[1;92m-------------------- [DATABASE] -------------------\033[0m");
    this->datetime.begin(minutesUpdateMeasures > 240 ? 240 : minutesUpdateMeasures);
    this->jsonArrayMeasures = jsonDocumentMeasures.to<JsonArray>();

    this->serverAddress = address;
    this->serverPort = port;

    this->maxAttempts = maxAttempts;

    this->isUpdated = true;

    while (!updateRoom()) { delay(1000); };
}

void ApiManagement::setCredentials(const String &serverUsername, const String &serverPassword) {
    this->serverUsername = serverUsername;
    this->serverPassword = serverPassword;
}

void ApiManagement::setRoomNumber(uint8_t roomNumber) { this->roomNumber = roomNumber; }

uint8_t ApiManagement::getRoomNumber() const { return roomNumber; }

bool ApiManagement::getIsUpdated() { return isUpdated; }

bool ApiManagement::updateRoom() {
    if (WiFi.status() == WL_CONNECTED) {
        if (login() == 200) {
            if (requestChangeStatusActivationRoom() == 200 && requestChangeLocalIpRoom(WiFi.localIP().toString()) == 200) {
                isUpdated = true;
            } else {
                isUpdated = false;
            }
        } else {
            isUpdated = false;
        }
    } else if (WiFi.status() == WL_DISCONNECTED) {
        isUpdated = false;
    } else {
        isUpdated = false;
    }

    return isUpdated;
}

int ApiManagement::login() {
    uint8_t countAttempts = 1;

    /* Login into the server and taking the token. */
    do {
        yield();

        uint16_t resultStatusCode = requestLogin();
        switch (resultStatusCode) {
            case 200:
                /* Storing the token for next purposes. */
                jsonDocumentLogin["token"]["tokenType"] = true;
                deserializeJson(jsonDocumentLogin, httpJsonResponse);
                serverToken = (String) jsonDocumentLogin["token"];
                serverTokenType = (String) jsonDocumentLogin["tokenType"];

                return resultStatusCode;
        }
    } while (countAttempts++ < maxAttempts);

    return 0;
}

bool ApiManagement::addMeasures(const String &timestamp, double temperature, double humidity) {
    /* Creating the JSON body. */
    if (jsonArrayMeasures.size() == 5) {
        jsonArrayMeasures.remove(0);
    }

    JsonObject measures = jsonArrayMeasures.createNestedObject();
    measures["when"] = timestamp;
    measures["room_number"] = roomNumber;
    measures["temperature"] = String(temperature, 2);
    measures["humidity"] = String(humidity, 2);

    String jsonDocumentMeasuresSerialized;
    serializeJson(jsonArrayMeasures, jsonDocumentMeasuresSerialized);

    /*
     * Check the connection and adding measures if there is connection and the login has been successful,
     *  or reconnecting in case of error.
     */
    if (WiFi.status() == WL_CONNECTED) {
        if (login() == 200) {
            if (isUpdated) {
                requestChangeLocalIpRoom(WiFi.localIP().toString());
            }

            requestSetMeasures(jsonDocumentMeasuresSerialized);

            Serial.println("\033[1;92m---------------- [TRANSACTION JSON] ---------------\033[0m");
            for (auto && jsonArrayMeasure : jsonArrayMeasures) {
                yield();

                JsonObject measure = jsonArrayMeasure.as<JsonObject>();
                Serial.println("\033[1;92mVALUES AT " + (String) measure["when"] + "\033[0m");
                Serial.println("\t\033[1;97mTEMPERATURE:   " + String(measure["temperature"]) + "\033[0m");
                Serial.println("\t\033[1;97mHUMIDITY:      " + String(measure["humidity"]) + "\033[0m");
            }
            Serial.println("\033[1;92m---------------------------------------------------\033[0m\n");

            jsonArrayMeasures.clear();
            jsonDocumentMeasures.garbageCollect();

            isUpdated = true;
            return isUpdated;
        }

        isUpdated = false;
        return isUpdated;
    }

    if (WiFi.status() == WL_DISCONNECTED) {
        Serial.println("\033[1;91m[WIFI ERROR FROM ApiManagement]\033[0m");
    }

    isUpdated = false;
    return isUpdated;
}

void ApiManagement::update() {
    if (datetime.checkDatetime()) {
        Serial.println("\033[1;92m-------------------- [DATABASE] -------------------\033[0m");

        addMeasures(datetime.getActualTimestamp(), sensor.getTemperature(), sensor.getHumidity());
        datetime.configNextDatetime();

        Serial.println("\033[1;96m[FREE HEAP SIZE: " + String(ESP.getFreeHeap()) + "]\033[0m");
    }
}

int ApiManagement::requestLogin() {
    httpClient.begin(wifiClient, serverAddress + ":" + serverPort + "/" + API_MANAGEMENT_URI_USER_LOGIN);
    httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int responseCode = httpClient.POST("username=" + serverUsername + "&password=" + serverPassword);
    httpJsonResponse = httpClient.getString();
    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + API_MANAGEMENT_URI_USER_LOGIN + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}

int ApiManagement::requestChangeStatusActivationRoom() {
    httpClient.begin(wifiClient, serverAddress + ":" + serverPort + "/" + API_MANAGEMENT_URI_ROOM_CHANGE_STATUS_ACTIVATION);
    httpClient.addHeader("Authorization", serverTokenType + " " + serverToken);
    httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int responseCode = httpClient.PATCH("number=" + String(roomNumber) + "&is_active=1");
    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + API_MANAGEMENT_URI_ROOM_CHANGE_STATUS_ACTIVATION + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}

int ApiManagement::requestChangeLocalIpRoom(const String &localIP) {
    httpClient.begin(wifiClient, serverAddress + ":" + serverPort + "/" + API_MANAGEMENT_URI_ROOM_API_CHANGE_LOCAL_IP);
    httpClient.addHeader("Authorization", serverTokenType + " " + serverToken);
    httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int responseCode = httpClient.PATCH("number=" + String(roomNumber) + "&local_ip=" + localIP);
    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + API_MANAGEMENT_URI_ROOM_API_CHANGE_LOCAL_IP + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}

int ApiManagement::requestSetMeasures(const String &jsonDocumentMeasuresSerialized) {
    httpClient.begin(wifiClient, serverAddress + ":" + serverPort + "/" + API_MANAGEMENT_URI_MEASURE_SET);
    httpClient.addHeader("Authorization", serverTokenType + " " + serverToken);
    httpClient.addHeader("Content-Type", "application/json");
    httpClient.addHeader("Accept", "application/json");
    int responseCode = httpClient.POST(jsonDocumentMeasuresSerialized);
    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + API_MANAGEMENT_URI_MEASURE_SET + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}