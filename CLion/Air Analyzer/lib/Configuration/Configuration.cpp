#include "Configuration.h"

void configurationVersion1(Button &button, Screen &screen) {
    int8_t resultButton = 0;

    /* Reset EEPROM to avoid some conflict. */
    resetEEPROM(SIZE_EEPROM);

    // Room ID
    uint8_t roomID = MIN_ROOM_NUMBER;
    do {
        yield();

        resultButton = button.checkPress();
        if (resultButton == 1) {
            if (roomID == MAX_ROOM_NUMBER) {
                roomID = MIN_ROOM_NUMBER;
            } else {
                roomID++;
            }
        }

        screen.setRoomNumber(roomID);
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

    serverSocket.begin(SERVER_SOCKET_PORT);
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

void configurationLoad(FirmwareUpdateOTA firmwareUpdateOta, ServerSocketJSON &serverSocket, Sensor &sensor, Screen &screen, ApiManagement &apiManagement, String &wifiSSID, String &wifiPassword) {
    unsigned long timeStartedLoadingMessage;
    float percentageLoadingMessage = (float) 100 / (((float) sizeof(loadingPageMessages) / sizeof(loadingPageMessages[0])) - 1);
    uint8_t iLoadingMessages = 0;

    uint8_t roomID = MIN_ROOM_NUMBER;
    char c_wifiSSID[SIZE_WIFI_SSID];
    char c_wifiPassword[SIZE_WIFI_PASSWORD];
    char c_credentialUsername[SERVER_SOCKET_SIZE_USERNAME];
    char c_credentialPassword[SERVER_SOCKET_SIZE_PASSWORD];

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
    serverSocket.begin(SERVER_SOCKET_PORT);
    delay(calculateDelay((long) timeStartedLoadingMessage, TIME_LOADING_MESSAGE));

    // FIRMWARE
    iLoadingMessages++;
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * (float) iLoadingMessages));
    firmwareUpdateOta.begin(FIRMWARE_UPDATE_OTA_BASE_ADDRESS, FIRMWARE_UPDATE_OTA_BASE_PORT);
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
    apiManagement.begin(API_MANAGEMENT_BASE_ADDRESS, API_MANAGEMENT_BASE_PORT, API_MANAGEMENT_MAX_ATTEMPTS, API_MANAGEMENT_MINUTES_UPDATE_MEASURES);
    delay(calculateDelay((long) timeStartedLoadingMessage, TIME_LOADING_MESSAGE));

    // Sensor
    iLoadingMessages++;
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * (float) iLoadingMessages));
    sensor.begin();
    delay(calculateDelay((long) timeStartedLoadingMessage, TIME_LOADING_MESSAGE));

    // Screen
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * (float) iLoadingMessages));
    screen.setRoomNumber(roomID);
    screen.setIsConnected(WiFi.isConnected());
    screen.setIsUpdated(apiManagement.getIsUpdated());
}