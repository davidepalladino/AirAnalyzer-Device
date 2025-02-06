#include <EEPROM.h>

#include <Button.h>
#include <Screen.h>

/**
* @brief Displays the brand information and allows the user to request a system reset.
 *
 * This function displays the brand and version information on the screen for a set duration (defined by `TIME_LOGO`).
 * During this time, it checks if the user presses the button to trigger a system reset. If the button is pressed,
 * the system will reset the EEPROM values and show a reset completion message on the screen.
 *
 * @param button The Button object used to detect user interaction (button press).
 * @param screen The Screen object used to display the brand information and messages.
 * @param version The version string of the system, which is displayed as part of the brand information.
 * @param addressVersionEEPROM The address in EEPROM where is located the version, to replace with `0`.
 * @param timeLogo The time in milliseconds for showing the logo.
 * @param timeMessageReset The time in milliseconds for showing the reset message.
 * @warning EEPROM must be already opened before calling this function, otherwise the reset will not work.
 */

void showBrand(Button &button, Screen &screen, String &version, uint8_t addressVersionEEPROM, uint16_t timeLogo, uint16_t timeMessageReset);