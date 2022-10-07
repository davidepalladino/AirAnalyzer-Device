 /**
  * @brief This library allows to manage the screen for the Air Analyzer purposes.
  * The class is an observer where updates own status when is notified by the subject class.
  * Copyright (c) 2022 Davide Palladino.
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact davidepalladino@hotmail.com
  * @website https://davidepalladino.github.io/
  * @version 1.5.0
  * @date 7th October, 2022
  * 
  */

#ifndef SCREEN_H
    #define SCREEN_H

    #include <Arduino.h>

    #include <U8g2lib.h>

    #include <AbstractObserver.h>
    #include <Sensor.h>

    #include "configScreen.h"
    #include "iconsScreen.h"

    class Sensor;

    class Screen : private AbstractObserver {
        friend class AbstractSubject;

        public:
            /** 
             * @brief This constructor creates the object setting only the subject class and the pins where is
             *  connected the screen.
             * @param sensor Subject object that will notify.
             * @param pinSCL Pin SCL where is connected the screen.
             * @param pinSDA Pin SDA where i connected the screen.
             */
            Screen(Sensor &sensor, uint8_t pinSCL, uint8_t pinSDA);

            /** 
             * @brief This method initializes the screen object.
             */
            void begin();

            /**
             * @brief This method sets the room ID.
             * @param roomNumber Number of room.
             */
            void setRoomNumber(uint8_t roomNumber);

            /**
             * @brief This method sets the status indicator about the WiFi connection.
             * @param isConnected Value "true" if there is a WiFi connection; else, value "false".
             */
            void setIsConnected(bool isConnected);

            /**
             * @brief This method gets the status indicator about the WiFi connection.
             * @return Value "true" if there is a WiFi connection; else, value "false".
             */
            bool getIsConnected();

            /**
             * @brief This method sets the status indicator about the update.
             * @param isUpdated Value "true" if the update has been successful; else, value "false".
             */
            void setIsUpdated(bool isUpdated);

            /**
             * @brief This method gets the status indicator about the update.
             * @return Value "true" if the update has been successful; else, value "false".
             */
            bool getIsUpdated();

            /**
             * @brief This method sets the status of screen, if can be rendered or not.
             * @param isViewable Value "true" if the screen can be rendered.
             */
            void setIsViewable(bool isViewable);

            /**
             * @brief This method gets the status of screen, if can be rendered or not.
             * @return
             */
            bool getIsViewable();



        // INSTALLATION and CONFIGURATION VIEWS
            /**
             * @brief This methods shows an installation page about the room ID.
             * @param messages Array of three messages to visualize.
             */
            void showInstallationRoomIDPage(const String messages[3]);

            /**
             * @brief This methods shows an installation page about the WiFi.
             * @param messages Array of six messages to visualize.
             * @param result Type of rows to visualize: "0" for three rows considering the first, second and third message;
             *  "1" considering the fourth and fifth messages ; "2" considering the sixth message.
             */
            void showInstallationWiFiPage(const String messages[6], uint8_t result);

            /**
             * @brief This methods shows an configuration page about the update two version.
             * @param messages Array of three messages to visualize. To the last message will be append the "localIP" value.
             * @param localIP Value selected and that must be show.
             */
            void showUpgradeVersionThreePage(const String *messages, const String &localIP);



            // ORDINARY VIEWS
            /**
             * @brief This method shows the brand and the version of the system.
             * @param version Version of the system.
             */
             void showBrand(const String &version);

            /**
             * @brief This method shows a page with an loading bar and a description message.
             * @param message The specific message to visualize.
             * @param percentage Percentage to fill the loading bar.
             */
            void showLoadingPage(const String &message, float percentage);

            /**
             * @brief This method shows the main page using the sensor object (to visualize the values) the room ID
             *  and the status of connection and update.
             */
            void showMainPage();

            /**
             * @brief This method clear the page with all contents.
             */
            void clear();

            /**
             * @brief This method shows a simple page with a simple row of message at the center.
             * @param message The specific row message to visualize.
             * @warning The row of message shall be of maximum 18 character. Other characters may be lost at right of screen.
             */
            void showMessagePage(const String &message);

            /**
             * @brief This method shows a simple page with two simple rows of messages at the center.
             * @param messages The two messages to visualize.
             * @warning The single row of message shall be of maximum 18 character. Other characters may be lost at right of screen.
             */
            void showMessagePage(const String messages[2]);
            
        private:
            U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C *screen;
            Sensor &sensor;
            uint8_t roomNumber;
            bool isConnected;
            bool isUpdated;
            bool isViewable;

            /** 
             * @brief This method draws the brand.
             */
            void drawBrand();

            /** 
             * @brief This method draws the version of the system.
             * @param version Version of the system.
             */
            void drawVersion(const String &version);

            /** 
             * @brief This method draws the bar and the internal fill considering the percenage.
             * @param percentage Percentage to fill the loading bar.
             */
            void drawBar(float percentage);

            /**
             * @brief This method draws the single message considering the "x" and "y" positions.
             * @param positionX X position.
             * @param positionY Y position.
             * @param message The specific row message to visualize.
             * @warning The row of message shall be of maximum 18 character. Other characters may be lost at right of screen.
             */
            void drawMessage(uint8_t positionX, uint8_t positionY, const String &message);

            /**
             * @brief This method draws a circle with the room ID inside.
             */
            void drawRoomID();

            /**
             * @brief This method draws the icon of temperature and the value read.
             */
            void drawTemperature();

            /**
             * @brief This method draws the icon of humidity and the value read.
             */
            void drawHumidity();

            /**
             * @brief This method draws the status icon if the connection has been successful.
             */
            void drawWiFiStatus();

            /**
             * @brief This method draws the status icon if the update has not been successful.
             */
            void drawUpdateStatus();

            /** 
             * @brief This method update the main screen.
             */
            void update();
    };
#endif