#include "ConfigurationUtils.h"

void resetEEPROM(uint16_t sizeEEPROM) {
    for (uint16_t a = 0; a <= sizeEEPROM; a++) {
        EEPROM.write(a, 0);
    }
    EEPROM.commit();
}

void forceConnectWiFi(const String &wifiSSID, const String &wifiPassword, uint8_t roomID) {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(wifiSSID, wifiPassword);
        WiFi.hostname("Air Analyzer-" + String(roomID));
        Serial.print("\nConnection to WiFi..");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\n\tLocal IP: " + WiFi.localIP().toString());
        Serial.println("\tHostname: " + WiFi.hostname());
        Serial.println("\tMode: " + String(WiFi.getMode()));
    }
}

void socketRetrieveCredentials(String jsonRequestSerialized, ApiManagement &apiManagement) {
    StaticJsonDocument<512> jsonDocumentRequest;
    deserializeJson(jsonDocumentRequest, jsonRequestSerialized);

    char c_username[SERVER_SOCKET_SIZE_USERNAME];
    char c_password[SERVER_SOCKET_SIZE_PASSWORD];
    static_cast<String>(jsonDocumentRequest[SERVER_SOCKET_FIELD_MESSAGE][SERVER_SOCKET_FIELD_MESSAGE_USERNAME]).toCharArray(c_username, SERVER_SOCKET_SIZE_USERNAME);
    static_cast<String>(jsonDocumentRequest[SERVER_SOCKET_FIELD_MESSAGE][SERVER_SOCKET_FIELD_MESSAGE_PASSWORD]).toCharArray(c_password, SERVER_SOCKET_SIZE_PASSWORD);

    EEPROM.put(ADDRESS_CREDENTIAl_USERNAME, c_username);
    EEPROM.put(ADDRESS_CREDENTIAL_PASSWORD, c_password);
    EEPROM.commit();

    apiManagement.setCredentials(String(c_username), String(c_password));
}

void socketSendRoomID(ServerSocketJSON &serverSocket, uint8_t roomID) {
    if (serverSocket.isAttached()) {
        StaticJsonDocument<512> jsonDocumentRequest;
        jsonDocumentRequest["RoomID"] = roomID;

        String jsonRequestSerialized;
        serializeJson(jsonDocumentRequest, jsonRequestSerialized);

        serverSocket.speak(jsonRequestSerialized);
    }
}
long calculateDelay(long timeStarted, long timeNecessary) {
    long difference = (long) (millis() - timeStarted);
    return (timeNecessary - difference) < 0 ? 0 : (timeNecessary - difference);
}
