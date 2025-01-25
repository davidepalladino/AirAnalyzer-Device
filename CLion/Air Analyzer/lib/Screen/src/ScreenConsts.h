const uint8_t positionBrand[2] =                                        {15, 20};                       // x, y
const uint8_t positionVersion[2] =                                      {15, 30};                       // x, y
const uint8_t positionLogoTemperature[2] =                              {12, 0};                        // x, y
const uint8_t positionLogoHumidity[2] =                                 {12, 18};                       // x, y
const uint8_t positionLogoWiFi[2] =                                     {120, 32};                      // x, y
const uint8_t positionLogoError[2] =                                    {110, 32};                      // x, y
const uint8_t positionValueTemperature[2] =                             {30, 12};                       // x, y
const uint8_t positionValueHumidity[2] =                                {30, 30};                       // x, y
const uint8_t positionValueRoomID[3] =                                  {108, 106, 15};                 // x, y, y
const uint8_t positionUnitTemperature[2] =                              {68, 12};                       // x, y
const uint8_t positionUnitHumidity[2] =                                 {68, 30};                       // x, y
const uint8_t positionFrameLoadingPage[2] =                             {10, 10};                       // x, y
const uint8_t positionCircleRoomID[2] =                                 {110, 10};                      // x, y
const uint8_t positionMessageInstallationRoomIDPage[4] =                {0, 8, 19, 30};                 // x, y, y, y
const uint8_t positionMessageInstallationWiFiPageCase0[4] =             {0, 8, 19, 30};                 // x, y, y, y
const uint8_t positionMessageInstallationWiFiPageCase1[3] =             {0, 12, 23};                    // x, y, y
const uint8_t positionMessageInstallationWiFiPageCase2[2] =             {0, 19};                        // x, y
const uint8_t positionMessageUpgradeVersionTwoPage[5] =                 {0, 26, 8, 19, 30};             // x, x, y, y, y
const uint8_t positionMessageLoadingPage[2] =                           {10, 28};                       // x, y
const uint8_t positionMessageMessagePage[2] =                           {10, 20};                       // x, y
const uint8_t sizeFrameLoadingPage[2] =                                 {108, 5};                       // width, height
const uint8_t radiusCircleRoomID =                                      10;

const uint8_t logoTemperatureWidth =                                    14;
const uint8_t logoTemperatureHeight =                                   14;
const uint8_t logoHumidityWidth =                                       14;
const uint8_t logoHumidityHeight =                                      14;

const String brandPageMessage =                                         "Air Analyzer";
const String messagePageResetCompleteMessages =                         "RESET COMPLETE";
const String messagePageSocketRequest =                                 "EXTERNAL REQUEST";
const String messagePageFirmwareUpdated[2] =                            {"FIRMWARE UPDATED", "RESTARTING..." };
const String installationRoomIDPageMessages[3] =                        {"Short press to", "change the ID. ", "Long press to finish."};
const String installationRoomWiFiPageMessages[6] =                      {"Search error, no", "connection revealed.", "Try again!", "Short press to start", "WPS connection.", "Searching..."};
const String upgradeConfigurationToVersionTwoMessages[2] =              {"Put this IP in your", "app to complete:"};
const String loadingPageMessages[6] =                                   {"GETTING EEPROM", "CONNECTING WIFI", "CHECKING FIRMWARE", "SETTING DATABASE", "SETTING SENSOR", "SETTING SCREEN"};
const String messagePageInstallationCompleteMessages[2] =               {"INSTALLATION", "COMPLETE"};
const String messagePageSearchingMessage =                              {"WPS CONNECTION..."};
const String messagePageSuccessfulMessage =                             {"CONNECTED!"};
const String messagePageErrorMessages[2] =                              {"NO NEW CONNECTION", "REVEALED!"};

static unsigned char logoTemperature[] PROGMEM = {
    0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01, 0xe0, 0x01,
    0xe0, 0x01, 0xe0, 0x01, 0xf0, 0x03, 0xf8, 0x07, 0xf8, 0x07, 0xf8, 0x07,
    0xf0, 0x03, 0xe0, 0x01 };

static unsigned char logoHumidity[] PROGMEM = {
    0xc0, 0x00, 0xe0, 0x01, 0xf0, 0x03, 0xf8, 0x07, 0xf8, 0x07, 0xfc, 0x0f,
    0xfc, 0x0f, 0xfc, 0x0f, 0xfe, 0x1f, 0xfe, 0x1f, 0xfc, 0x0f, 0xfc, 0x0f,
    0xf8, 0x07, 0xf0, 0x03 };