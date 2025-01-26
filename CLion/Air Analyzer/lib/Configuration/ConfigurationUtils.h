#include <EEPROM.h>

#include <ArduinoJson.h>

#include <ApiManagement.h>
#include <Button.h>
#include <FirmwareUpdateOTA.h>
#include <Sensor.h>
#include <ServerSocketJSON.h>
#include <Screen.h>

#include "ConfigurationConsts.h"

/**
 * @brief Resets the EEPROM cells to the value "0".
 *
 * This function iterates through the specified number of EEPROM cells and sets their values to "0".
 * It is useful for clearing stored data or resetting device configuration.
 *
 * @param sizeEEPROM The number of EEPROM cells to reset.
 */
void resetEEPROM(uint16_t sizeEEPROM);

/**
 * @brief Forces a Wi-Fi connection using the provided credentials.
 *
 * This function attempts to connect to a Wi-Fi network with the specified SSID and password.
 * It also generates a unique hostname based on the provided Room ID.
 *
 * @param wifiSSID The SSID (name) of the Wi-Fi network.
 * @param wifiPassword The password required to join the Wi-Fi network.
 * @param roomID The room ID, which is used to create a unique hostname.
 */
void forceConnectWiFi(const String &wifiSSID, const String &wifiPassword, uint8_t roomID);

/**
 * @brief Stores user credentials from a JSON message into the EEPROM.
 *
 * This function deserializes the provided JSON string, extracts user credentials,
 * and stores them in EEPROM. The credentials can later be used for API interactions.
 *
 * @param jsonRequestSerialized The JSON message containing user credentials to store.
 * @param apiManagement An instance of the API management object used to set the credentials.
 * @warning EEPROM must be already opened before calling this function.
 */
void socketRetrieveCredentials(String jsonRequestSerialized, ApiManagement &apiManagement);

/**
 * @brief Sends the Room ID to the client via a server socket.
 *
 * This function sends the specified Room ID to a connected client through a server socket.
 * The server socket must be open, and the client must be connected to receive the message.
 *
 * @param serverSocket The object representing the server socket used for communication.
 * @param roomID The Room ID to be sent to the client.
 * @warning Ensure the server socket is open and the client is connected before calling this function.
 */
void socketSendRoomID(ServerSocketJSON &serverSocket, uint8_t roomID);

/**
 * @brief Calculates the delay based on elapsed time and the required duration.
 * 
 * This function computes the remaining time to complete a loading action. It is typically used to display a loading message or animation.
 * If the elapsed time is greater than the required time, the function returns "0" to indicate completion.
 * 
 * @param timeStarted The timestamp when the timer started.
 * @param timeNecessary The required time for the operation (e.g., loading duration).
 * @return The remaining time or "0" if the elapsed time exceeds the required time.
 */
long calculateDelay(long timeStarted, long timeNecessary);