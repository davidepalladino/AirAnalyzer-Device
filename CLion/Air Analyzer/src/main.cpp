 /**
  * This software allows to manage the hardware of "Air Analyzer". Specifically, you can read the values about temperature and humidity, 
  *  storing them into database for extern analysis.
  * Copyright (c) 2020 Davide Palladino. 
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact me@davidepalladino.com
  * @website www.davidepalladino.com
  * @version 3.0.0
  * @date 9th August, 2021
  * 
  */

#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <WiFiUdp.h>
#include <EEPROM.h>

#include <NTPClient.h>

#include <ServerSocketJSON.h>
#include <Button.h>
#include <Sensor.h>
#include <Screen.h>
#include <DatetimeInterval.h>
#include <DatabaseManagement.h>
#include <FirmwareUpdateOTA.h>

#include "functionSetup.h"  // This includes "globalSettings.h" too.

FirmwareUpdateOTA firmwareUpdate;
ServerSocketJSON serverSocket;
Button button(PIN_BUTTON, B_PULLUP, TIME_LONG_PRESS);
Sensor sensor(ADDRESS_SENSOR, HUMIDITY_RESOLUTION, TEMPERATURE_RESOLUTION);
Screen screen(sensor, PIN_SCL, PIN_SDA);

NTPClient ntpClient(*new WiFiUDP(), (long) 0);
DatabaseManagement database(sensor, *(new DatetimeInterval(ntpClient)));

String wifiSSID;
String wifiPassword;
uint8_t requestCodeSocket = 0;
int8_t resultButton = 0;
unsigned long timeoutSaveEEPROM = 0;
bool errorSavingDatabase = false;

void setup() {
    Serial.begin(BAUDRATE);
    screen.begin();
    EEPROM.begin(SIZE_EEPROM);

    uint8_t actualVersionEEPROM = 0;

    /* Showing brand, with version, and checking if is requested of reset. */
    showBrand(button, screen, (String&) VERSION_FIRMWARE);
    Serial.println("\nVersion firmware: " + VERSION_FIRMWARE);

    /* Checking the version of data on EEPROM, to execute the right update of EEPROM and/or system. */
    EEPROM.get(ADDRESS_VERSION_EEPROM, actualVersionEEPROM);
    switch (actualVersionEEPROM) {
        /* If this is a first utilization about the system, will be launched the installation. */
        case 0:
            configurationVersion1(button, screen);

        /* Else if is a first or the second version, will be launched the upgrade to version 3. */
        case 1:
        case 2:
            configurationVersion3(serverSocket, screen, database);

            /*
             * Showing a message of complete.
             * This will have to show after the last update implemented.
             */
            screen.showMessagePage(messagePageInstallationCompleteMessages);
            delay(TIME_MESSAGE);

        default:
            configurationLoad(firmwareUpdate, serverSocket,sensor, screen, database, wifiSSID, wifiPassword);
    }
    EEPROM.end();
}

void loop() {
    /* Checking if the device is connected like server. */
    if (serverSocket.isConnected()) {
        serverSocket.attachClient();

        requestCodeSocket = serverSocket.listen();
        if (requestCodeSocket > 0) {
            unsigned long timeStartedMessage;

            switch (requestCodeSocket) {
                case 1:
                    timeStartedMessage = millis();
                    screen.showMessagePage(messagePageSocketRequest);

                    EEPROM.begin(SIZE_EEPROM);
                    socketRetrieveCredentials(serverSocket.getJsonRequestSerialized(), database);
                    EEPROM.end();

                    delay(calculateDelay((long) timeStartedMessage, TIME_MESSAGE));
                    break;

                default:
                    break;
            }
        }
    } else {
        serverSocket.end();
        serverSocket.begin(PORT_SERVER_SOCKET);
    }

    /* 
     * Checking the status of button and execution the right action:
     *  - "-1" to execute WPS connection;
     *  - "1" to change the room ID.
     */
    resultButton = button.checkPress();
    if (resultButton == -1) {
      char c_wifiSSID[SIZE_WIFI_SSID];
      char c_wifiPassword[SIZE_WIFI_PASSWORD];

      screen.showMessagePage(messagePageSearchingMessage);

      bool result = WiFi.beginWPSConfig(); 
      if (result) {
        if (WiFi.SSID().length() > 0) {
          wifiSSID = WiFi.SSID();
          wifiSSID.toCharArray(c_wifiSSID, SIZE_WIFI_SSID);
          wifiPassword = WiFi.psk();
          wifiPassword.toCharArray(c_wifiPassword, SIZE_WIFI_PASSWORD);

          EEPROM.begin(SIZE_EEPROM);
          EEPROM.put(ADDRESS_WIFI_SSID, c_wifiSSID);
          EEPROM.put(ADDRESS_WIFI_PASSWORD, c_wifiPassword);
          EEPROM.commit();
          EEPROM.end();

          screen.showMessagePage(messagePageSuccessfulMessage);
          delay(TIME_MESSAGE);
        } else {
          WiFi.begin(wifiSSID, wifiPassword);

          screen.showMessagePage(messagePageErrorMessages);
          delay(TIME_MESSAGE);
        }
      }
    } else if (resultButton == 1) {
      if (database.getRoomID() == MAX_ROOM_ID) {
        database.setRoomID(MIN_ROOM_ID);
      } else {
        database.setRoomID(database.getRoomID() + 1);
      }
      screen.setRoomID(database.getRoomID());
      screen.showMainPage();

      timeoutSaveEEPROM = millis() + TIME_SAVE_EEPROM;
    }

    /* Saving on EEPROM and updating the database only if the time is elapsed. */
    if ((timeoutSaveEEPROM < millis()) && (timeoutSaveEEPROM != 0)) {
      timeoutSaveEEPROM = 0;

      errorSavingDatabase = !database.updateRoom();

      EEPROM.begin(SIZE_EEPROM);
      if (EEPROM.read(ADDRESS_ROOM_ID) != database.getRoomID()) {
        EEPROM.write(ADDRESS_ROOM_ID, database.getRoomID());
        EEPROM.commit();
      }
      EEPROM.end();
    }

    /* Updating the status icons on the screen if there is a change. */
    if (screen.getIsUpdated() != database.getIsUpdated()) {
        screen.setIsUpdated(database.getIsUpdated());
        screen.showMainPage();
    }  

    if (screen.getIsConnected() != WiFi.isConnected()) {
        screen.setIsConnected(WiFi.isConnected());
        screen.showMainPage();
    } 

    /* If there was an error on previous saving of the new room ID into database, there will be a new attempt. */
    if (errorSavingDatabase) {
      errorSavingDatabase = !database.updateRoom();
    }       

    sensor.check();
}