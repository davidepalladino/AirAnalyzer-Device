#include <ApiManagement.h>

ApiManagement::ApiManagement(
    Sensor &sensor,
    DatetimeInterval &datetime,
    const String& address,
    uint16_t port,
    const String& uriUserLogin,
    const String& uriRoomChangeStatusActivation,
    const String& uriRoomChangeLocalIp,
    const String& uriMeasureSet,
    uint8_t maxAttempts,
    uint16_t minutesUpdate
) : sensor(sensor), datetime(datetime) {
    this->address = address;
    this->port = port;
    this->uriUserLogin = uriUserLogin;
    this->uriRoomChangeStatusActivation = uriRoomChangeStatusActivation;
    this->uriRoomChangeLocalIp = uriRoomChangeLocalIp;
    this->uriMeasureSet = uriMeasureSet;
    this->maxAttempts = maxAttempts;
    this->minutesUpdate = minutesUpdate;

    this->sensor.addObserver(this);
}

void ApiManagement::begin() {
    Serial.println("\033[1;92m-------------------- [DATABASE] -------------------\033[0m");
    this->datetime.begin(minutesUpdate > 240 ? 240 : minutesUpdate);
    this->jsonArrayMeasures = jsonDocumentMeasures.to<JsonArray>();

    this->isUpdated = true;

    while (!updateRoom()) { delay(1000); };
}

void ApiManagement::setCredentials(const String &serverUsername, const String &serverPassword) {
    this->username = serverUsername;
    this->password = serverPassword;
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
                deserializeJson(jsonDocumentLogin, response);
                token = (String) jsonDocumentLogin["token"];
                tokenType = (String) jsonDocumentLogin["tokenType"];

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
    serializeJson(measures, jsonDocumentMeasuresSerialized);

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
        } else {
            isUpdated = false;
            return isUpdated;
        }
    } else if (WiFi.status() == WL_DISCONNECTED) {
        Serial.println("\033[1;91m[WIFI ERROR FROM ApiManagement]\033[0m");

        isUpdated = false;
        return isUpdated;
    } else {
        isUpdated = false;
        return isUpdated;
    }
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
    httpClient.begin(wifiClient, address + ":" + port + "/" + uriUserLogin);
    httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int responseCode = httpClient.POST("username=" + username + "&password=" + password);
    response = httpClient.getString();
    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + uriUserLogin + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}

int ApiManagement::requestChangeStatusActivationRoom() {
    httpClient.begin(wifiClient, address + ":" + port + "/" + uriRoomChangeStatusActivation);
    httpClient.addHeader("Authorization", tokenType + " " + token);
    httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int responseCode = httpClient.PATCH("number=" + String(roomNumber) + "&is_active=1");
    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + uriRoomChangeStatusActivation + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}

int ApiManagement::requestChangeLocalIpRoom(const String &localIP) {
    httpClient.begin(wifiClient, address + ":" + port + "/" + uriRoomChangeLocalIp);
    httpClient.addHeader("Authorization", tokenType + " " + token);
    httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int responseCode = httpClient.PATCH("number=" + String(roomNumber) + "&local_ip=" + localIP);
    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + uriRoomChangeLocalIp + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}

int ApiManagement::requestSetMeasures(const String &jsonDocumentMeasuresSerialized) {
    httpClient.begin(wifiClient, address + ":" + port + "/" + uriMeasureSet);
    httpClient.addHeader("Authorization", tokenType + " " + token);
    httpClient.addHeader("Content-Type", "application/json");
    httpClient.addHeader("Accept", "application/json");
    int responseCode = httpClient.POST(jsonDocumentMeasuresSerialized);
    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + uriMeasureSet + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}