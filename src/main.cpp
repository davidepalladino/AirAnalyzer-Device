/**
 * This software allows to manage the hardware of "Air Analyzer". Specifically, you can read the values about temperature and humidity,
 *  storing them into apiManagement for extern analysis.
 * Copyright (c) 2025 Davide Palladino.
 * All right reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 5.0.0
 * @date 6th February 2025
 *
 */

#include <Configuration.h>
#include <SensorObserver.h>

#include "utils.h"
#include "settings.h"

FirmwareUpdateOTA firmwareUpdate;
ServerSocketJSON serverSocket;
Button button(BUTTON_PIN, B_PULLUP, BUTTON_TIME_LONG_PRESS);
Sensor sensor(SENSOR_ADDRESS, SENSOR_HUMIDITY_RESOLUTION, SENSOR_TEMPERATURE_RESOLUTION);
Screen screen(SCREEN_PIN_SCL, SCREEN_PIN_SDA);

NTPClient ntpClient(*new WiFiUDP(), (long) 0);
ApiManagement apiManagement(*(new DatetimeInterval(ntpClient)));

String wifiSSID;
String wifiPassword;
uint8_t requestCodeSocket = 0;
int8_t resultButton = 0;
unsigned long timeoutSaveEEPROM = 0;
unsigned long timeoutTurnOffScreen = 0;
unsigned long timeoutTurnOnScreen = 0;
bool errorSavingDatabase = false;

constexpr uint16_t TIME_TURN_OFF = 3000;  // Don't change this value.
constexpr uint16_t TIME_TURN_ON = 100;    // Don't change this value.

void setup() {
    Serial.begin(BAUDRATE);
    screen.begin();
    EEPROM.begin(SIZE_EEPROM);

    uint8_t actualVersionEEPROM = 0;

    /* Adding observers to Sensor. */
    sensor.addObserver(&apiManagement);
    sensor.addObserver(&screen);

    /* Showing brand, with version, and checking if is requested of reset. */
    showBrand(button, screen, const_cast<String&>(VERSION_FIRMWARE), ADDRESS_VERSION_EEPROM, TIME_LOGO, TIME_MESSAGE);
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
            configurationVersion3(serverSocket, screen, apiManagement);

            /*
             * Showing a message of complete.
             * This will have to show after the last update implemented.
             */
            screen.showMessagePage(messagePageInstallationCompleteMessages);
            delay(TIME_MESSAGE);

        default:
            configurationLoad(firmwareUpdate, serverSocket, sensor, screen, apiManagement, wifiSSID, wifiPassword);
    }
    EEPROM.end();

    screen.showMainPage();

    timeoutTurnOffScreen = millis() + TIME_TURN_OFF;
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
                    socketRetrieveCredentials(serverSocket.getJsonRequestSerialized(), apiManagement);
                    EEPROM.end();

                    delay(calculateDelay(static_cast<long>(timeStartedMessage), TIME_MESSAGE));
                    break;

                default:
                    break;
            }
        }
    } else {
        serverSocket.end();
        serverSocket.begin(SERVER_SOCKET_PORT);
    }

    /* 
     * Checking the status of button and execution the right action:
     *  - "-1" to execute WPS connection;
     *  - "1" to show the screen or change the room number, if the screen is already turned on.
     */
    resultButton = button.checkPress();
    if (resultButton == -1) {
        screen.showMessagePage(messagePageSearchingMessage);

        if (WiFi.beginWPSConfig()) {
            if (WiFi.SSID().length() > 0) {
                char c_wifiPassword[SIZE_WIFI_PASSWORD];
                char c_wifiSSID[SIZE_WIFI_SSID];

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
        if (apiManagement.getRoomNumber() == MAX_ROOM_NUMBER) {
            apiManagement.setRoomNumber(MIN_ROOM_NUMBER);
        } else {
            apiManagement.setRoomNumber(apiManagement.getRoomNumber() + 1);
        }
        screen.setRoomNumber(apiManagement.getRoomNumber());
        screen.showMainPage();
    }

    /* Saving on EEPROM and updating the apiManagement only if the time is elapsed. */
    if ((timeoutSaveEEPROM < millis()) && (timeoutSaveEEPROM != 0)) {
        timeoutSaveEEPROM = 0;

        errorSavingDatabase = !apiManagement.updateRoom();

        EEPROM.begin(SIZE_EEPROM);
            if (EEPROM.read(ADDRESS_ROOM_ID) != apiManagement.getRoomNumber()) {
            EEPROM.write(ADDRESS_ROOM_ID, apiManagement.getRoomNumber());
            EEPROM.commit();
        }
        EEPROM.end();
    }

    /* Updating the status icons on the screen if there is a change. */
    if (screen.isUpdated() != apiManagement.isUpdated()) {
        screen.isUpdated(apiManagement.isUpdated());
        screen.showMainPage();
    }  

    if (screen.isConnected() != WiFi.isConnected()) {
        screen.isConnected(WiFi.isConnected());
        screen.showMainPage();
    } 

    /* If there was an error on previous saving of the new room ID into apiManagement, there will be a new attempt. */
    if (errorSavingDatabase) {
        errorSavingDatabase = !apiManagement.updateRoom();
    }

    /* Protecting the screen by applying standby and recovering after certain time. */
    if ((timeoutTurnOffScreen < millis()) && (timeoutTurnOffScreen != 0)) {
        screen.clear();

        timeoutTurnOffScreen = 0;
        timeoutTurnOnScreen = millis() + TIME_TURN_ON;
    } else if ((timeoutTurnOnScreen < millis()) && (timeoutTurnOnScreen != 0)) {
        screen.showMainPage();

        timeoutTurnOffScreen = millis() + TIME_TURN_OFF;
        timeoutTurnOnScreen = 0;
    }

    sensor.check();
}