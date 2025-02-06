#pragma once

#include "ConfigurationUtils.h"

/**
 * @brief Handles the initial system installation, guiding the user through room ID and Wi-Fi configuration,
 * and storing the obtained data into EEPROM.
 *
 * This function performs the first-time setup of the system, allowing the user to configure essential parameters
 * such as the room ID and Wi-Fi credentials. The provided button is used to navigate through the steps,
 * while the screen displays status messages to assist the user throughout the process.
 *
 * ## Steps performed by this function:
 * 1. **EEPROM Reset:** Clears the EEPROM to avoid potential conflicts with previously stored data.
 * 2. **Room ID Setup:**
 *    - Displays the room ID configuration screen.
 *    - The user can cycle through valid room numbers using the button.
 *    - Selection is finalized when the button is held for a long press.
 * 3. **Wi-Fi Setup:**
 *    - Prompts the user to initiate WPS (Wi-Fi Protected Setup).
 *    - If the credentials are successfully retrieved, they are stored in EEPROM.
 *    - If the process fails, the user is prompted to retry.
 * 4. **EEPROM Commit:**
 *    - Saves the collected data (version, Wi-Fi credentials, and room ID) into EEPROM for persistent storage.
 *
 * @param button Reference to the Button object used for user interaction during installation.
 * @param screen Reference to the Screen object used for displaying messages and installation steps.
 *
 * @warning The EEPROM must be initialized (opened) before calling this function to ensure proper operation.
 *
 * @note The function uses the WPS feature to automatically configure Wi-Fi credentials.
 *
 * @attention The function includes blocking calls such as `yield()` and `delay()`, which might affect real-time performance.
 *
 * @return None (void)
 */
void configurationVersion1(Button &button, Screen &screen);

/**
 * @brief Upgrades the system to version 3 by storing Wi-Fi credentials using an Android app via socket communication.
 *
 * This function facilitates the upgrade process, enabling the device to receive Wi-Fi credentials and
 * a room ID from an Android application through a server socket connection. It also handles EEPROM remapping
 * for devices previously running version 2, ensuring compatibility with the new memory structure.
 *
 * ## Steps performed by this function:
 * 1. **EEPROM Version Check and Remapping:**
 *    - If the stored EEPROM version is detected as 2, the function retrieves old credentials from specific addresses.
 *    - EEPROM is reset to avoid conflicts, and the retrieved data is stored at new addresses.
 * 2. **Retrieving Stored Credentials:**
 *    - Existing Wi-Fi credentials and room ID are read from EEPROM.
 * 3. **Wi-Fi and Server Initialization:**
 *    - The function forces a Wi-Fi connection using stored credentials and displays the device's IP address on the screen.
 *    - A server socket is started to listen for incoming credentials from the Android app.
 * 4. **Credential Retrieval via Socket:**
 *    - The function listens for incoming requests from the Android app.
 *    - Once credentials are received, they are processed and stored.
 * 5. **Finalizing Upgrade:**
 *    - The EEPROM is updated with the new version number to signify a successful upgrade.
 *
 * @param serverSocket Reference to the ServerSocketJSON object to open a server socket and communicate with the Android app.
 * @param screen Reference to the Screen object used to visualize the status of the upgrade process.
 * @param apiManagement Reference to the ApiManagement object for handling the received credentials.
 *
 * @warning The EEPROM must be initialized (opened) before calling this function to ensure proper operation.
 *
 * @note This function blocks execution until the credentials are successfully received.
 *
 * @attention Ensure the Wi-Fi network is available and the Android app is ready to provide credentials via socket communication.
 *
 * @return None (void)
 */
void configurationVersion3(ServerSocketJSON &serverSocket, Screen &screen, ApiManagement &apiManagement);

/**
 * @brief Loads the system configuration and initializes essential components.
 *
 * This function handles the loading process by retrieving stored configurations from EEPROM,
 * establishing Wi-Fi and server connections, checking for firmware updates, and initializing
 * API and sensor components. It provides visual feedback on a screen to indicate progress.
 *
 * ## Steps performed by this function:
 * 1. **EEPROM Data Retrieval:**
 *    - Reads Wi-Fi credentials, room ID, and API credentials from EEPROM.
 *    - Converts stored character arrays to `String` objects for easier handling.
 * 2. **Wi-Fi and Server Setup:**
 *    - Establishes a connection to the stored Wi-Fi network.
 *    - Starts the server socket to listen for client connections.
 * 3. **Firmware Update Check:**
 *    - Initializes the OTA (Over-the-Air) firmware update module.
 *    - Checks if a firmware update is available and restarts the device if necessary.
 * 4. **API Initialization:**
 *    - Configures the API management module with the room number and stored credentials.
 *    - Starts the API communication with the defined server address and update parameters.
 * 5. **Sensor Initialization:**
 *    - Starts the sensor to begin collecting data.
 * 6. **Final UI Setup:**
 *    - Updates the screen with the current room number, Wi-Fi connection status, and API update status.
 *
 * @param firmwareUpdateOta Reference to the FirmwareUpdateOTA object responsible for handling OTA updates.
 * @param serverSocket Reference to the ServerSocketJSON object for opening a server socket.
 * @param sensor Reference to the Sensor object to initialize sensor readings.
 * @param screen Reference to the Screen object used to visualize the status of the configuration loading.
 * @param apiManagement Reference to the ApiManagement object to handle API communications.
 * @param wifiSSID Reference to a String object to store the retrieved Wi-Fi SSID.
 * @param wifiPassword Reference to a String object to store the retrieved Wi-Fi password.
 *
 * @warning The EEPROM must be initialized (opened) before calling this function to avoid runtime errors.
 *
 * @note The function provides real-time progress updates on the screen and manages delays to synchronize operations.
 *
 * @attention Ensure the system is connected to power and within Wi-Fi range to prevent failures during loading.
 *
 * @return None (void)
 */
void configurationLoad(FirmwareUpdateOTA firmwareUpdateOta, ServerSocketJSON &serverSocket, Sensor &sensor, Screen &screen, ApiManagement &apiManagement, String &wifiSSID, String &wifiPassword);