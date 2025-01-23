/**
 * @file Screen.h
 * @brief Provides functionality to manage the screen for Air Analyzer purposes.
 *
 * This library enables screen management as an observer, updating its state when notified by the subject class.
 *
 * Copyright (c) 2025 Davide Palladino.
 * All rights reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 2.0.0
 * @date 23rd January 2025
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <AbstractObserver.h>
#include <Sensor.h>
#include "config.h"
#include "icons.h"

class Sensor;

/**
 * @class Screen
 * @brief Manages screen operations and updates for the Air Analyzer system.
 *
 * This class observes the Sensor class and updates the screen accordingly.
 */
class Screen : private AbstractObserver {
    friend class AbstractSubject;

    public:
        /**
         * @brief Constructs a Screen object.
         * @param sensor Reference to the Sensor object.
         * @param pinSCL SCL pin connected to the screen.
         * @param pinSDA SDA pin connected to the screen.
         */
        Screen(Sensor &sensor, uint8_t pinSCL, uint8_t pinSDA);

        /** @brief Initializes the screen. */
        void begin();

        /** @brief Sets the room ID. */
        void setRoomNumber(uint8_t roomNumber);

        /** @brief Sets and gets the WiFi connection status. */
        void setIsConnected(bool isConnected);
        bool getIsConnected();

        /** @brief Sets and gets the update status. */
        void setIsUpdated(bool isUpdated);
        bool getIsUpdated();

        /** @brief Sets and gets the screen visibility status. */
        void setIsViewable(bool isViewable);
        bool getIsViewable();

        // INSTALLATION AND CONFIGURATION VIEWS
        void showInstallationRoomIDPage(const String messages[3]);
        void showInstallationWiFiPage(const String messages[6], uint8_t result);
        void showUpgradeVersionThreePage(const String *messages, const String &localIP);

        // ORDINARY VIEWS
        void showBrand(const String &version);
        void showLoadingPage(const String &message, float percentage);
        void showMainPage();
        void clear();
        void showMessagePage(const String &message);
        void showMessagePage(const String messages[2]);

    private:
        U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C *screen;
        Sensor &sensor;
        uint8_t roomNumber;
        bool isConnected;
        bool isUpdated;
        bool isViewable;

        void drawBrand();
        void drawVersion(const String &version);
        void drawBar(float percentage);
        void drawMessage(uint8_t positionX, uint8_t positionY, const String &message);
        void drawRoomID();
        void drawTemperature();
        void drawHumidity();
        void drawWiFiStatus();
        void drawUpdateStatus();
        void update();
};

#endif // SCREEN_H
