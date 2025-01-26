#include "utils.h"

void showBrand(Button &button, Screen &screen, String &version, uint8_t addressVersionEEPROM, uint16_t timeLogo, uint16_t timeMessageReset) {
    unsigned long timeoutLogo = millis() + timeLogo;
    int8_t resultButton = 0;

    screen.showBrand(version);
    while (millis() <= timeoutLogo) {
        yield();

        /* Checking if there is a request to reset. */
        resultButton = button.checkPress();
        if (resultButton == -1) {
            EEPROM.put(addressVersionEEPROM, 0);
            EEPROM.commit();

            screen.showMessagePage(messagePageResetCompleteMessages);
            delay(timeMessageReset);

            break;
        }
    }
}