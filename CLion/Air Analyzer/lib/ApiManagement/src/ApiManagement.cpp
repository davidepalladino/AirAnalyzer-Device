#include <ApiManagement.h>

ApiManagement::ApiManagement(Sensor &sensor, DatetimeInterval &datetime) : sensor(sensor), datetime(datetime) {
    this->sensor.addObserver(this);
}

void ApiManagement::begin(const String &address, uint16_t port, uint8_t nAttempt, uint8_t timeoutMinutes) {
    Serial.println("\033[1;92m-------------------- [DATABASE] -------------------\033[0m");
    this->datetime.begin(timeoutMinutes > 240 ? 240 : timeoutMinutes);
    this->jsonArrayMeasures = jsonDocumentMeasures.to<JsonArray>();

    this->serverAddress = address;
    this->serverPort = port;

    this->nAttempts = nAttempt;

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
            static std::map<String, String> headersChangeStatusActivationRoom;
            headersChangeStatusActivationRoom.insert(std::pair<String, String>("Content-Type", "application/x-www-form-urlencoded"));
            headersChangeStatusActivationRoom.insert(std::pair<String, String>("Authorization", serverTokenType + " " + serverToken));

            static std::map<String, String> bodyChangeStatusActivationRoom;
            bodyChangeStatusActivationRoom.insert(std::pair<String, String>("number", this->roomNumber));
            bodyChangeStatusActivationRoom.insert(std::pair<String, String>("is_active", 1));

            static std::map<String, String> headersChangeLocalIpRoom;
            headersChangeLocalIpRoom.insert(std::pair<String, String>("Content-Type", "application/x-www-form-urlencoded"));
            headersChangeLocalIpRoom.insert(std::pair<String, String>("Authorization", serverTokenType + " " + serverToken));

            static std::map<String, String> bodyChangeLocalIpRoom;
            bodyChangeLocalIpRoom.insert(std::pair<String, String>("number", this->roomNumber));
            bodyChangeLocalIpRoom.insert(std::pair<String, String>("local_ip", (WiFi.localIP().toString())));

            if (
                    requestPatch(API_CHANGE_STATUS_ACTIVATION_ROOM, headersChangeStatusActivationRoom, bodyChangeStatusActivationRoom) == 200 &&
                    requestPatch(API_CHANGE_LOCAL_IP_ROOM, headersChangeLocalIpRoom, bodyChangeLocalIpRoom) == 200
                ) {
                isUpdated = true;
            } else {
                isUpdated = false;
            }

            headersChangeStatusActivationRoom.clear();
            bodyChangeStatusActivationRoom.clear();
            headersChangeLocalIpRoom.clear();
            bodyChangeLocalIpRoom.clear();
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

uint16_t ApiManagement::login() {
    uint8_t countAttempts = 1;

    static std::map<String, String> headers;
    headers.insert(std::pair<String, String>("Content-Type", "application/x-www-form-urlencoded"));

    static std::map<String, String> body;
    body.insert(std::pair<String, String>("username", serverUsername));
    body.insert(std::pair<String, String>("password", serverPassword));

    /* Login into the server and taking the token. */
    do {
        yield();

        uint16_t resultStatusCode = requestPost(API_LOGIN, headers, body);
        switch (resultStatusCode) {
            case 200:
                /* Storing the token for next purposes. */
                jsonDocumentLogin["token"]["tokenType"] = true;
                deserializeJson(jsonDocumentLogin, httpJsonResponse);
                serverToken = (String) jsonDocumentLogin["token"];
                serverTokenType = (String) jsonDocumentLogin["tokenType"];

                headers.clear();
                body.clear();

                return resultStatusCode;
        }
    } while (countAttempts++ < nAttempts);

    headers.clear();
    body.clear();

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
                static std::map<String, String> headersChangeLocalIpRoom;
                headersChangeLocalIpRoom.insert(std::pair<String, String>("Content-Type", "application/x-www-form-urlencoded"));
                headersChangeLocalIpRoom.insert(std::pair<String, String>("Authorization", serverTokenType + " " + serverToken));

                static std::map<String, String> bodyChangeLocalIpRoom;
                bodyChangeLocalIpRoom.insert(std::pair<String, String>("number", this->roomNumber));
                bodyChangeLocalIpRoom.insert(std::pair<String, String>("local_ip", (WiFi.localIP().toString())));

                Serial.println(3);

                requestPatch(API_CHANGE_LOCAL_IP_ROOM, headersChangeLocalIpRoom, bodyChangeLocalIpRoom);

                headersChangeLocalIpRoom.clear();
                bodyChangeLocalIpRoom.clear();
            }

            Serial.println(4);

            static std::map<String, String> headersMeasures;
            headersMeasures.insert(std::pair<String, String>("Authorization", serverTokenType + " " + serverToken));
            headersMeasures.insert(std::pair<String, String>("Content-Type", "application/json"));

            Serial.println(5);

            requestPost(API_SET_MEASURES, headersMeasures, jsonDocumentMeasuresSerialized);

            headersMeasures.clear();

            Serial.println(6);

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

int ApiManagement::requestPost(String uri, std::map<String, String> headers, std::map<String, String> body) {
    httpClient.begin(wifiClient, serverAddress + ":" + serverPort + "/" + uri);

    std::map<String, String>::iterator iteratorHeader;
    for (iteratorHeader = headers.begin(); iteratorHeader != headers.end(); ++iteratorHeader ) {
        httpClient.addHeader(iteratorHeader->first, iteratorHeader->second);
    }

    String bodyString = "";
    std::map<String, String>::iterator iteratorBody;
    for (iteratorBody = body.begin(); iteratorBody != body.end(); ++iteratorBody ) {
        bodyString += iteratorBody->first + "=" +  iteratorBody->second + "&";
    }

    bodyString = bodyString.substring(0, bodyString.length() - 1);

    int responseCode = httpClient.POST(bodyString);
    httpJsonResponse = httpClient.getString();

    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + uri + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}

int ApiManagement::requestPost(String uri, std::map<String, String> headers, String body) {
    httpClient.begin(wifiClient, serverAddress + ":" + serverPort + "/" + uri);

    std::map<String, String>::iterator iteratorHeader;
    for (iteratorHeader = headers.begin(); iteratorHeader != headers.end(); ++iteratorHeader ) {
        httpClient.addHeader(iteratorHeader->first, iteratorHeader->second);
    }

    int responseCode = httpClient.POST(body);
    httpJsonResponse = httpClient.getString();

    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + uri + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}

int ApiManagement::requestPatch(String uri, std::map<String, String> headers, std::map<String, String> body) {
    httpClient.begin(wifiClient, serverAddress + ":" + serverPort + "/" + uri);

    std::map<String, String>::iterator iteratorHeader;
    for (iteratorHeader = headers.begin(); iteratorHeader != headers.end(); ++iteratorHeader ) {
        httpClient.addHeader(iteratorHeader->first, iteratorHeader->second);
    }

    String bodyString = "";
    std::map<String, String>::iterator iteratorBody;
    for (iteratorBody = body.begin(); iteratorBody != body.end(); ++iteratorBody ) {
        bodyString += iteratorBody->first + "=" +  iteratorBody->second + "&";
    }

    int responseCode = httpClient.PATCH(bodyString);
    httpJsonResponse = httpClient.getString();

    httpClient.end();

    Serial.println("\033[1;96m[RESPONSE FOR " + uri + ": " + String(responseCode) + "]\033[0m\n");

    return responseCode;
}