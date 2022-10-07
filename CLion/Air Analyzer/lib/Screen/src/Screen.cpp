#include <Screen.h>

Screen::Screen(Sensor& sensor, uint8_t pinSCL, uint8_t pinSDA) : sensor(sensor) {
    sensor.addObserver(this);
    screen = new U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C (U8G2_R0, pinSCL, pinSDA);

    roomNumber = 0;

    isConnected = true;
    isUpdated = false;
}

void Screen::begin() { screen->begin(); }

void Screen::setRoomNumber(uint8_t roomNumber) { this->roomNumber = roomNumber; }

void Screen::setIsConnected(bool isConnected) { this->isConnected = isConnected; }

bool Screen::getIsConnected() { return isConnected; }

void Screen::setIsUpdated(bool isUpdated) { this->isUpdated = isUpdated; }

bool Screen::getIsUpdated() { return isUpdated; }

void Screen::setIsViewable(bool isViewable) { this->isViewable = isViewable; }

bool Screen::getIsViewable() { return isViewable; }


// INSTALLATION and CONFIGURATION VIEWS
void Screen::showInstallationRoomIDPage(const String messages[3]) {
    screen->firstPage();
    do {
        drawRoomID();
        drawMessage(positionMessageInstallationRoomIDPage[0], positionMessageInstallationRoomIDPage[1], messages[0]);
        drawMessage(positionMessageInstallationRoomIDPage[0], positionMessageInstallationRoomIDPage[2], messages[1]);
        drawMessage(positionMessageInstallationRoomIDPage[0], positionMessageInstallationRoomIDPage[3], messages[2]);
    } while (screen->nextPage());
}

void Screen::showInstallationWiFiPage(const String messages[6], uint8_t result) {
    screen->firstPage();
    do {
        drawWiFiStatus();
        switch (result) {
            case 0:
                drawMessage(positionMessageInstallationWiFiPageCase0[0], positionMessageInstallationWiFiPageCase0[1], messages[0]);
                drawMessage(positionMessageInstallationWiFiPageCase0[0], positionMessageInstallationWiFiPageCase0[2], messages[1]);
                drawMessage(positionMessageInstallationWiFiPageCase0[0], positionMessageInstallationWiFiPageCase0[3], messages[2]);
                break;
            case 1:
                drawMessage(positionMessageInstallationWiFiPageCase1[0], positionMessageInstallationWiFiPageCase1[1], messages[3]);
                drawMessage(positionMessageInstallationWiFiPageCase1[0], positionMessageInstallationWiFiPageCase1[2], messages[4]);
                break;
            case 2:
                drawMessage(positionMessageInstallationWiFiPageCase2[0], positionMessageInstallationWiFiPageCase2[1], messages[5]);
                break;
            default:
                break;
        }
    } while (screen->nextPage());
}
void Screen::showUpgradeVersionThreePage(const String *messages, const String &localIP) {
    screen->firstPage();
    do {
        drawMessage(positionMessageUpgradeVersionTwoPage[0], positionMessageUpgradeVersionTwoPage[2], messages[0]);
        drawMessage(positionMessageUpgradeVersionTwoPage[0], positionMessageUpgradeVersionTwoPage[3], messages[1]);
        drawMessage(positionMessageUpgradeVersionTwoPage[1], positionMessageUpgradeVersionTwoPage[4], localIP);
    } while (screen->nextPage());
}



// ORDINARY VIEWS
void Screen::showBrand(const String &version) {
    screen->firstPage();
    do {
        drawBrand();
        drawVersion(version);
    } while (screen->nextPage());
}

void Screen::showLoadingPage(const String &message, float percentage) {
    screen->firstPage();
    do {
        drawBar(percentage);
        drawMessage(positionMessageLoadingPage[0], positionMessageLoadingPage[1], message);
    } while (screen->nextPage());
}

void Screen::showMainPage() {
    screen->firstPage();
    do {
        drawRoomID();
        drawTemperature();
        drawHumidity();
        drawWiFiStatus();
        drawUpdateStatus();
    } while (screen->nextPage());
}

void Screen::clear() { screen->clear(); }

void Screen::showMessagePage(const String &message) {
    screen->firstPage();
    do {
        drawMessage(positionMessageMessagePage[0], positionMessageMessagePage[1], message);
    } while (screen->nextPage());
}

void Screen::showMessagePage(const String messages[2]) {
    screen->firstPage();
    do {
        drawMessage(positionMessageMessagePage[0], positionMessageMessagePage[1] - 6, messages[0]);
        drawMessage(positionMessageMessagePage[0], positionMessageMessagePage[1] + 6, messages[1]);
    } while (screen->nextPage());
}

void Screen::drawBrand() {
    screen->setFont(u8g2_font_smart_patrol_nbp_tf);
    screen->setCursor(positionBrand[0], positionBrand[1]);
    screen->print(brandPageMessage);
}

void Screen::drawVersion(const String &version) {
    screen->setFont(u8g2_font_profont11_mf);
    screen->setCursor(positionVersion[0], positionVersion[1]);
    screen->print("v" + version);
}

void Screen::drawBar(float percentage) {
    uint8_t widthFill = map((long) percentage, 0, 100, 0, sizeFrameLoadingPage[0]);
    screen->drawFrame(positionFrameLoadingPage[0], positionFrameLoadingPage[1], sizeFrameLoadingPage[0], sizeFrameLoadingPage[1]);
    screen->drawBox(positionFrameLoadingPage[0], positionFrameLoadingPage[1], widthFill, sizeFrameLoadingPage[1]);
}

void Screen::drawMessage(uint8_t positionX, uint8_t positionY, const String &message) {
    screen->setFont(u8g2_font_profont11_mf);
    screen->drawStr(positionX, positionY, message.c_str());
}

void Screen::drawRoomID() {
    screen->drawCircle(positionCircleRoomID[0], positionCircleRoomID[1], radiusCircleRoomID, U8G2_DRAW_ALL);
    screen->setFont(u8g2_font_smart_patrol_nbp_tf);
    if (roomNumber == 1) {
        screen->setCursor(positionValueRoomID[0], positionValueRoomID[2]);
    } else {
        screen->setCursor(positionValueRoomID[1], positionValueRoomID[2]);
    }
    screen->print(roomNumber);
}

void Screen::drawTemperature() {
    screen->drawXBMP(positionLogoTemperature[0], positionLogoTemperature[1], logoTemperatureWidth, logoTemperatureHeight, logoTemperature);
    screen->setFont(u8g2_font_smart_patrol_nbp_tf);
    screen->setCursor(positionValueTemperature[0], positionValueTemperature[1]);
    screen->print(sensor.getTemperature(), 1);
    screen->setCursor(positionUnitTemperature[0], positionUnitTemperature[1]);
    screen->print(String((char) 176) + "C");
}

void Screen::drawHumidity() {
    screen->drawXBMP(positionLogoHumidity[0], positionLogoHumidity[1], logoHumidityWidth, logoHumidityHeight, logoHumidity);
    screen->setFont(u8g2_font_smart_patrol_nbp_tf);
    screen->setCursor(positionValueHumidity[0], positionValueHumidity[1]);
    screen->print(sensor.getHumidity(), 1);
    screen->setCursor(positionUnitHumidity[0], positionUnitHumidity[1]);
    screen->print(" %");   
}

void Screen::drawWiFiStatus() {
    if (isConnected) {
        screen->setFont(u8g2_font_open_iconic_www_1x_t);
        screen->drawGlyph(positionLogoWiFi[0], positionLogoWiFi[1], 0x0048); 
    }
}

void Screen::drawUpdateStatus() {
    if (!isUpdated) {
        screen->setFont(u8g2_font_open_iconic_embedded_1x_t);
        screen->drawGlyph(positionLogoError[0], positionLogoError[1], 0x0047); 
    }
}

void Screen::update() {
    if (isViewable) {
        showMainPage();
    }
}