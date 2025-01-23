#include "FirmwareUpdateOTA.h"

FirmwareUpdateOTA::FirmwareUpdateOTA(const String& address, const uint16_t port, const String& uriGetLatest) {
    this->address = address;
    this->port = port;
    this->uriGetLatest = uriGetLatest;
}

void FirmwareUpdateOTA::begin() {
    Serial.println("\033[1;92m-------------------- [FIRMWARE] -------------------\033[0m");
    ESPhttpUpdate.rebootOnUpdate(false);
}

bool FirmwareUpdateOTA::check(const String &version) {
    t_httpUpdate_return ret = ESPhttpUpdate.update(wifiClient, address + ":" + String(port) + "/" + uriGetLatest, version);
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