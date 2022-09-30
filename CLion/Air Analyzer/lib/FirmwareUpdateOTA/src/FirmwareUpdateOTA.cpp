#include "FirmwareUpdateOTA.h"

FirmwareUpdateOTA::FirmwareUpdateOTA() {
}

void FirmwareUpdateOTA::begin(const String &address, uint16_t port, const String &uri) {
    Serial.println("\033[1;92m-------------------- [FIRMWARE] -------------------\033[0m");

    this->serverAddress = address;
    this->serverPort = port;

    this->serverUri = uri;

    ESPhttpUpdate.rebootOnUpdate(false);
}

bool FirmwareUpdateOTA::check(const String &version) {
    t_httpUpdate_return ret = ESPhttpUpdate.update(wifiClient, serverAddress + ":" + String(serverPort) + "/" + serverUri, version);
    switch (ret) {
        case HTTP_UPDATE_OK:
            Serial.println("\033[1;92m[FIRMWARE UPDATED]\033[0m");
            return true;

        case HTTP_UPDATE_FAILED:
            Serial.println("\033[1;91m[ERROR UPDATE FIRMWARE]\033[0m");
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("\033[1;93m[NO UPDATE FIRMWARE]\033[0m");
            break;
    }

    Serial.println("\033[1;92m---------------------------------------------------\033[0m");

    return false;
}