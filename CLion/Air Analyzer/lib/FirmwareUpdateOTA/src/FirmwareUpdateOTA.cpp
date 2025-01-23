#include "FirmwareUpdateOTA.h"

FirmwareUpdateOTA::FirmwareUpdateOTA() { }

void FirmwareUpdateOTA::begin() {
    Serial.println("\033[1;92m-------------------- [FIRMWARE] -------------------\033[0m");

    this->serverAddress = FIRMWARE_UPDATE_OTA_BASE_URL;
    this->serverPort = FIRMWARE_UPDATE_OTA_BASE_PORT;

    this->serverUri = FIRMWARE_UPDATE_OTA_URI_GET_LATEST;

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