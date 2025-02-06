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
 * @version 5.0.0
 * @date 6th February 2025
 */

#ifndef SCREEN_H
    #define SCREEN_H

    #include <Arduino.h>
    #include <U8g2lib.h>
    #include <Sensor.h>

    #include "ScreenConsts.h"

    /**
     * @class Screen
     * @brief Manages screen operations and updates for the Air Analyzer system.
     *
     * This class observes the Sensor class and updates the screen accordingly.
     */
    class Screen : public SensorObserver {
        public:
            /**
             * @brief Constructs a Screen object.
             *
             * @param pinSCL SCL pin connected to the screen (for I2C communication).
             * @param pinSDA SDA pin connected to the screen (for I2C communication).
             */
            Screen(uint8_t pinSCL, uint8_t pinSDA);

            /**
             * @brief Initializes the screen display.
             *
             * This method should be called before displaying any content on the screen.
             */
            void begin();

            /**
             * @brief Sets the room ID to be displayed on the screen.
             *
             * @param roomNumber The ID of the room.
             */
            void setRoomNumber(uint8_t roomNumber);

            /**
             * @brief Sets the Wi-Fi connection status.
             *
             * @param isConnected True if connected, false otherwise.
             */
            void isConnected(bool isConnected);

            /**
             * @brief Gets the Wi-Fi connection status.
             *
             * @return True if connected, false otherwise.
             */
            bool isConnected();

            /**
             * @brief Sets the update status of the system.
             * 
             * @param isUpdated True if updated, false otherwise.
             */
            void isUpdated(bool isUpdated);

            /**
             * @brief Gets the update status of the system.
             * 
             * @return True if updated, false otherwise.
             */
            bool isUpdated();

            // VIEWS for installation and configuration
            /**
             * @brief Displays the installation page for room ID setup.
             * 
             * @param messages An array of messages to display.
             */
            void showInstallationRoomIDPage(const String messages[3]);

            /**
             * @brief Displays the installation page for Wi-Fi setup.
             * 
             * @param messages An array of messages to display.
             * @param result A status code representing the Wi-Fi connection result.
             */
            void showInstallationWiFiPage(const String messages[6], uint8_t result);

            /**
             * @brief Displays the firmware upgrade version screen.
             * 
             * @param messages Pointer to an array of messages.
             * @param localIP The local IP address to display.
             */
            void showUpgradeVersionThreePage(const String *messages, const String &localIP);


            // VIEWS ordinary
            /**
             * @brief Displays the brand logo along with the firmware version.
             * 
             * @param version The firmware version string to be displayed.
             */
            void showBrand(const String &version);

            /**
             * @brief Displays a loading page with progress.
             * 
             * @param message The loading message to display.
             * @param percentage The loading percentage value.
             */
            void showLoadingPage(const String &message, float percentage);

            /**
             * @brief Displays the main screen page with the current sensor data.
             *
             * This method updates the main screen to show the latest temperature and humidity readings.
             * It is typically called when new sensor data is available and needs to be presented
             * to the user in a clear and user-friendly format.
             *
             * @param temperature The current temperature value to be displayed (in degrees Celsius).
             * @param humidity The current humidity value to be displayed (as a percentage).
             *
             * @note This method assumes the screen has already been initialized and is ready for updates.
             */
            void showMainPage(double temperature, double humidity);

            /** @brief Clears the screen display. */
            void clear();

            /**
             * @brief Displays a message on the screen.
             * 
             * @param message The message to display.
             */
            void showMessagePage(const String &message);

            /**
             * @brief Displays two messages on the screen.
             * 
             * @param messages An array containing two messages to display.
             */
            void showMessagePage(const String messages[2]);

        private:
            U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C *screen;         /**< Pointer to the screen object. */
            uint8_t roomNumber;                                     /**< Stores the room number. */
            bool connectionState;                                   /**< Stores the Wi-Fi connection status. */
            bool updateState;                                       /**< Stores the update status. */

            /** @brief Draws the brand logo on the screen. */
            void drawBrand();

            /**
             * @brief Draws the firmware version on the screen.
             * 
             * @param version The firmware version string to display.
             */
            void drawVersion(const String &version);

            /**
             * @brief Draws a progress bar on the screen.
             * 
             * @param percentage The loading percentage to be displayed.
             */
            void drawBar(float percentage);

            /**
             * @brief Draws a message at the specified position on the screen.
             * 
             * @param positionX X-coordinate of the message.
             * @param positionY Y-coordinate of the message.
             * @param message The message to display.
             */
            void drawMessage(uint8_t positionX, uint8_t positionY, const String &message);

            /** @brief Draws the room ID on the screen. */
            void drawRoomID();

            /**
            * @brief Draws the current temperature value on the screen.
            *
            * This method updates the specific area of the screen dedicated to displaying the temperature.
            * The temperature is typically presented in degrees Celsius, formatted for readability.
            *
            * @param temperature The temperature value to be displayed (in degrees Celsius).
            *
            * @note The screen must be initialized and set to the appropriate display page before calling this method.
            */
            void drawTemperature(double temperature);

            /**
             * @brief Draws the current humidity value on the screen.
             *
             * This method updates the specific area of the screen dedicated to displaying the humidity.
             * The humidity is typically presented as a percentage.
             *
             * @param humidity The humidity value to be displayed (as a percentage).
             *
             * @note The screen must be initialized and set to the appropriate display page before calling this method.
             */
            void drawHumidity(double humidity);

            /** @brief Draws the Wi-Fi connection status icon. */
            void drawWiFiStatus();

            /** @brief Draws the update status icon. */
            void drawUpdateStatus();

            /**
             * @brief Updates the screen content based on the latest sensor data.
             *
             * This method refreshes the display to show the updated temperature and humidity values.
             * It is typically invoked when the observed sensor detects changes in its readings.
             *
             * @param temperature The updated temperature value (in degrees Celsius).
             * @param humidity The updated humidity value (as a percentage).
             *
             * @note
             * - The screen must be initialized and displaying the appropriate page before calling this method.
             * - Call this method only when new data is available from the sensor to avoid unnecessary updates.
             */
            void update(double temperature, double humidity) override;
    };

#endif // SCREEN_H
