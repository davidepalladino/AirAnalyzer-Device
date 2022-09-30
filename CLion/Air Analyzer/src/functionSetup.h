#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include <ArduinoJson.h>

#include <ServerSocketJSON.h>
#include <Button.h>
#include <Sensor.h>
#include <Screen.h>
#include <ApiManagement.h>

#include "../globalSettings.h"

#define MIN_ROOM_ID 1
#define MAX_ROOM_ID 9

void configurationVersion1(Button &button, Screen &screen);
void configurationVersion3(ServerSocketJSON &serverSocket, Screen &screen, ApiManagement &apiManagement);
void configurationLoad(FirmwareUpdateOTA firmwareUpdateOta, ServerSocketJSON &serverSocket, Sensor &sensor, Screen &screen, ApiManagement &apiManagement, String &wifiSSID, String &wifiPassword);
void socketRetrieveCredentials(String jsonRequestSerialized, ApiManagement &apiManagement);
void socketSendRoomID(ServerSocketJSON &serverSocket, uint8_t roomID);
void showBrand(Button &button, Screen &screen, String &version);
long calculateDelay(long timeStarted, long timeNecessary);
void resetEEPROM(uint16_t sizeEEPROM);
void forceConnectWiFi(const String &wifiSSID, const String &wifiPassword, uint8_t roomID);

/**
 * @brief This procedure provides the first installation about the system, getting the information and storing into EEPROM.
 * @param button Button to provide the user to execute the steps.
 * @param screen Screen to visualize the status of installation with the messages.
 * @warning EEPROM must be already opened.
 */
void configurationVersion1(Button &button, Screen &screen) {
    int8_t resultButton = 0;

    /* Reset EEPROM to avoid some conflict. */
    resetEEPROM(SIZE_EEPROM);

    // Room ID
    uint8_t roomID = MIN_ROOM_ID;
    do {
        yield();

        resultButton = button.checkPress();
        if (resultButton == 1) {
            if (roomID == MAX_ROOM_ID) {
                roomID = MIN_ROOM_ID;
            } else {
                roomID++;
            }
        }

        screen.setRoomID(roomID);
        screen.showInstallationRoomIDPage(installationRoomIDPageMessages);
    } while (resultButton != -1);

    // WiFi
    char wifiSSID[SIZE_WIFI_SSID];
    char wifiPassword[SIZE_WIFI_PASSWORD];
    do {
        yield();
        screen.showInstallationWiFiPage(installationRoomWiFiPageMessages, 1);
        do {
            yield();
            resultButton = button.checkPress();
        } while (resultButton != 1);

        screen.showInstallationWiFiPage(installationRoomWiFiPageMessages, 2);

        bool result = WiFi.beginWPSConfig(); 
        if (result) {
            if (WiFi.SSID().length() > 0) {
                WiFi.SSID().toCharArray(wifiSSID, SIZE_WIFI_SSID);
                WiFi.psk().toCharArray(wifiPassword, SIZE_WIFI_PASSWORD);

                break;
            } else {
                screen.showInstallationWiFiPage(installationRoomWiFiPageMessages, 3);
                delay(TIME_MESSAGE);
            }
        }

    } while (WiFi.status() != WL_CONNECTED);

    // EEPROM
    EEPROM.put(ADDRESS_VERSION_EEPROM, VERSION_EEPROM);
    EEPROM.put(ADDRESS_WIFI_SSID, wifiSSID);
    EEPROM.put(ADDRESS_WIFI_PASSWORD, wifiPassword);
    EEPROM.put(ADDRESS_ROOM_ID, roomID);
    EEPROM.commit();
}

/**
 * @brief This procedure provides the upgrade to version 3 about the system. In this upgrade will be stored the credentials
 *  with the Android APP, through a socket communication.
 * @param serverSocket Object to open a server socket.
 * @param screen Screen to visualize the status of upgrade with the messages.
 * @param apiManagement API object to set the credentials.
 * @warning EEPROM must be already opened.
 */
void configurationVersion3(ServerSocketJSON &serverSocket, Screen &screen, ApiManagement &apiManagement) {
    char wifiSSID[SIZE_WIFI_SSID];
    char wifiPassword[SIZE_WIFI_PASSWORD];
    uint8_t roomID = 0;
    bool isCredentialsRetrieved = false;
    uint8_t requestCodeSocket = 0;

    /* If the EEPROM version of device is set to 2, it means that must be remapped. */
    if (EEPROM.read(ADDRESS_VERSION_EEPROM) == 2) {
        /* Defining the old address of EEPROM. */
        const uint8_t OLD_ADDRESS_WIFI_SSID = 1;
        const uint8_t OLD_ADDRESS_WIFI_PASSWORD = 34;
        const uint8_t OLD_ADDRESS_ROOM_ID = 99;

        /* Getting the values stored and resetting the EEPROM. */
        EEPROM.get(OLD_ADDRESS_WIFI_SSID, wifiSSID);
        EEPROM.get(OLD_ADDRESS_WIFI_PASSWORD, wifiPassword);
        EEPROM.get(OLD_ADDRESS_ROOM_ID, roomID);
        resetEEPROM(SIZE_EEPROM);

        /* Storing the values previously stored. */
        EEPROM.put(ADDRESS_WIFI_SSID, wifiSSID);
        EEPROM.put(ADDRESS_WIFI_PASSWORD, wifiPassword);
        EEPROM.put(ADDRESS_ROOM_ID, roomID);
        EEPROM.commit();
    }

    /* Retrieving the values form EEPROM. */
    EEPROM.get(ADDRESS_WIFI_SSID, wifiSSID);
    EEPROM.get(ADDRESS_WIFI_PASSWORD, wifiPassword);
    EEPROM.get(ADDRESS_ROOM_ID, roomID);

    /* Forcing WiFi and server connection and showing an instruction message. */
    forceConnectWiFi(String(wifiSSID), String(wifiPassword), roomID);
    screen.showUpgradeVersionThreePage(upgradeConfigurationToVersionTwoMessages, WiFi.localIP().toString());

    serverSocket.begin(PORT_SERVER_SOCKET);
    while (!isCredentialsRetrieved) {
        serverSocket.attachClient();

        requestCodeSocket = serverSocket.listen();
        switch (requestCodeSocket) {
            case 1:
                socketRetrieveCredentials(serverSocket.getJsonRequestSerialized(), apiManagement);
                isCredentialsRetrieved = true;
                break;

            default:
                break;
        }

    }
    serverSocket.detachClient();

    /* Because the configuration is complete, the EEPROM will be updated with the new version number. */
    EEPROM.put(ADDRESS_VERSION_EEPROM, VERSION_EEPROM);
    EEPROM.commit();
}

/**
 * @brief This procedure provides the loading of configuration about the system.
 * @param firmwareUpdateOta Object to manage the firmware.
 * @param serverSocket Object to open a server socket.
 * @param sensor Sensor to initialize.
 * @param screen Screen to visualize the status of loading with the messages.
 * @param apiManagement API object to initialize.
 * @param wifiSSID Object to store the SSID of WiFi.
 * @param wifiPassword Object to store the password of WiFi.
 * @warning EEPROM must be already opened.
 */
void configurationLoad(FirmwareUpdateOTA firmwareUpdateOta, ServerSocketJSON &serverSocket, Sensor &sensor, Screen &screen, ApiManagement &apiManagement, String &wifiSSID, String &wifiPassword) {
    unsigned long timeStartedLoadingMessage;
    float percentageLoadingMessage = (float) 100 / (((float) sizeof(loadingPageMessages) / sizeof(loadingPageMessages[0])) - 1);
    uint8_t iLoadingMessages = 0;

    uint8_t roomID = MIN_ROOM_ID;
    char c_wifiSSID[SIZE_WIFI_SSID];
    char c_wifiPassword[SIZE_WIFI_PASSWORD];
    char c_credentialUsername[SIZE_CREDENTIAL_USERNAME];
    char c_credentialPassword[SIZE_CREDENTIAL_PASSWORD];

    // EEPROM
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * (float) iLoadingMessages));
    EEPROM.get(ADDRESS_WIFI_SSID, c_wifiSSID);
    EEPROM.get(ADDRESS_WIFI_PASSWORD, c_wifiPassword);
    EEPROM.get(ADDRESS_ROOM_ID, roomID);
    EEPROM.get(ADDRESS_CREDENTIAl_USERNAME, c_credentialUsername);
    EEPROM.get(ADDRESS_CREDENTIAL_PASSWORD, c_credentialPassword);
    wifiSSID = c_wifiSSID;
    wifiPassword = c_wifiPassword;
    delay(calculateDelay((long) timeStartedLoadingMessage, TIME_LOADING_MESSAGE));

    // WiFi
    iLoadingMessages++;
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * (float) iLoadingMessages));
    forceConnectWiFi(wifiSSID, wifiPassword, roomID);
    serverSocket.begin(PORT_SERVER_SOCKET);
    delay(calculateDelay((long) timeStartedLoadingMessage, TIME_LOADING_MESSAGE));

    // FIRMWARE
    iLoadingMessages++;
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * (float) iLoadingMessages));
    firmwareUpdateOta.begin(BASE_URL, BASE_PORT, FINGERPRINT, FIRMWARE_URI);
    if (firmwareUpdateOta.check(VERSION_FIRMWARE)) {
        screen.showMessagePage(messagePageFirmwareUpdated);
        ESP.restart();
    }
    delay(calculateDelay((long) timeStartedLoadingMessage, TIME_LOADING_MESSAGE));

    // API
    iLoadingMessages++;
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * (float) iLoadingMessages));
    apiManagement.setRoomNumber(roomID);
    apiManagement.setCredentials(String(c_credentialUsername), String(c_credentialPassword));
    apiManagement.begin(BASE_URL, BASE_PORT, FINGERPRINT, 3, API_MINUTES_UPDATE_MEASURES);
    delay(calculateDelay((long) timeStartedLoadingMessage, TIME_LOADING_MESSAGE));

    // Sensor
    iLoadingMessages++;
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * (float) iLoadingMessages));
    sensor.begin();
    delay(calculateDelay((long) timeStartedLoadingMessage, TIME_LOADING_MESSAGE));

    // Screen
    // Screen
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * (float) iLoadingMessages));
    screen.setRoomID(roomID);
    screen.setIsConnected(WiFi.isConnected());
    screen.setIsUpdated(apiManagement.getIsUpdated());
}

/**
 * @brief This procedures provides a force connection of WiFi.
 * @param wifiSSID SSID necessary to connect to WiFi.
 * @param wifiPassword Password necessary to connect to WiFi.
 * @param roomID Room ID necessary to create a singular hostname.
 */
void forceConnectWiFi(const String &wifiSSID, const String &wifiPassword, uint8_t roomID) {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(wifiSSID, wifiPassword);
        WiFi.hostname("Air Analyzer-" + String(roomID));
        Serial.print("\nConnection to WiFi..");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\n\tLocal IP: " + WiFi.localIP().toString());
        Serial.println("\tHostname: " + WiFi.hostname());
        Serial.println("\tMode: " + String(WiFi.getMode()));
    }
}

/**
 * @brief This procedures provides to store the user credential, contained into JSON message, to the EEPROM.
 * @param jsonRequestSerialized JSON message necessary to deserialize.
 * @param apiManagement API object to set the credentials.
 * @warning EEPROM must be already opened.
 */
void socketRetrieveCredentials(String jsonRequestSerialized, ApiManagement &apiManagement) {
    StaticJsonDocument<SIZE_JSON_DOCUMENT> jsonDocumentRequest;
    deserializeJson(jsonDocumentRequest, jsonRequestSerialized);

    char c_username[SIZE_CREDENTIAL_USERNAME];
    char c_password[SIZE_CREDENTIAL_PASSWORD];
    ((String) jsonDocumentRequest["Message"]["Username"]).toCharArray(c_username, SIZE_CREDENTIAL_USERNAME);
    ((String) jsonDocumentRequest["Message"]["Password"]).toCharArray(c_password, SIZE_CREDENTIAL_PASSWORD);

    EEPROM.put(ADDRESS_CREDENTIAl_USERNAME, c_username);
    EEPROM.put(ADDRESS_CREDENTIAL_PASSWORD, c_password);
    EEPROM.commit();

    apiManagement.setCredentials(String(c_username), String(c_password));
}

/**
 * @brief This procedures provides to send the Room ID to the client socket with the server socket.
 * @param serverSocket Object to open a server socket.
 * @param roomID Room ID to send to the client socket.
 * @warning The server socket will have to be opened and the client will have to be connected too.
 */
void socketSendRoomID(ServerSocketJSON &serverSocket, uint8_t roomID) {
    if (serverSocket.isAttached()) {
        StaticJsonDocument<SIZE_JSON_DOCUMENT> jsonDocumentRequest;
        jsonDocumentRequest["RoomID"] = roomID;

        String jsonRequestSerialized;
        serializeJson(jsonDocumentRequest, jsonRequestSerialized);

        serverSocket.speak(jsonRequestSerialized);
    }
}

/**
 * @brief This procedure provides the showing of the brand and the possibility request of reset.
 * @param button Button to provide the user to execute the steps.
 * @param screen Screen to visualize the status of installation with the messages.
 * @param version Version of the system.
 * @warning EEPROM must be already opened.
 */
void showBrand(Button &button, Screen &screen, String &version) {
    unsigned long timeoutLogo = millis() + TIME_LOGO;
    int8_t resultButton = 0;

    screen.showBrand(version);
    while (millis() <= timeoutLogo) {
        yield();

        /* Checking if there is a request to reset. */
        resultButton = button.checkPress();
        if (resultButton == -1) {
            EEPROM.put(ADDRESS_VERSION_EEPROM, 0);
            EEPROM.commit();

            screen.showMessagePage(messagePageResetCompleteMessages);
            delay(TIME_MESSAGE);

            break;
        }
    }
}

/**
 * @brief This function provides to calculate the delay based on the past time and the time necessary. Is used for the loading message during the loading page.
 * @param timeStarted Past time, where the timer is started.
 * @param timeNecessary Time necessary, where will be used to calculate the difference.
 * @return "0" if the difference between the past time and the present time is greater than time necessary; else the result of this difference with time necessary.
 */
long calculateDelay(long timeStarted, long timeNecessary) {
    long difference = (long) (millis() - timeStarted);
    return (timeNecessary - difference) < 0 ? 0 : (timeNecessary - difference);
}

/**
 * @brief This procedure provides to reset to value "0" the cells of EEPROM.
 * @param sizeEEPROM Number of cells to reset.
 */
void resetEEPROM(uint16_t sizeEEPROM) {
    for (uint16_t a = 0; a <= sizeEEPROM; a++) {
        EEPROM.write(a, 0);
    }
    EEPROM.commit();
}