#include "FirmwareUpdateOTA.h"

FirmwareUpdateOTA::FirmwareUpdateOTA() { }

void FirmwareUpdateOTA::begin(const String &address, uint16_t port) {
    Serial.println("\033[1;92m-------------------- [FIRMWARE] -------------------\033[0m");

    this->serverAddress = address;
    this->serverPort = port;

    ESPhttpUpdate.rebootOnUpdate(false);
}

bool FirmwareUpdateOTA::check(const String &version) {
    switch (ESPhttpUpdate.update(wifiClient, serverAddress + ":" + String(serverPort) + "/" + FIRMWARE_UPDATE_OTA_URI_GET_LATEST, version)) {
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