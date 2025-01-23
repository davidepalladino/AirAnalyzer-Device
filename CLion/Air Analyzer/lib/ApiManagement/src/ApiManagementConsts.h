const String API_MANAGEMENT_BASE_ADDRESS =                                  "http://airanalyzer.shadowmoses.ovh";
const uint8_t API_MANAGEMENT_BASE_PORT =                                    80;

const String API_MANAGEMENT_URI_USER_LOGIN =                                "api/user/login";
const String API_MANAGEMENT_URI_ROOM_CHANGE_STATUS_ACTIVATION =             "api/room/changeStatusActivation";
const String API_MANAGEMENT_URI_ROOM_API_CHANGE_LOCAL_IP =                  "api/room/changeLocalIP";
const String API_MANAGEMENT_URI_MEASURE_SET =                               "api/measure/set";

constexpr uint8_t API_MANAGEMENT_MAX_ATTEMPTS =                             3;
constexpr uint8_t API_MANAGEMENT_MINUTES_UPDATE_MEASURES =                  10;