# AirAnalyzer-Device
## Description
This repository contains the software for the Air Analyzer project, enabling users to:
- view current temperature and humidity readings;
- modify the room ID.

## Components Used
The project utilizes the following hardware components:
- **NodeMCU ESP8266 ESP-12F**, serves as the main MicroController Unit;
- **0.98" OLED Screen (I2C)**, displays system information (Wi-Fi status and error during update) and sensor data;
- **HDC1080 Sensor**: measures temperature and humidity levels;
- **RTC DS3231**: provides real-time clock functionality;
- **Push Button**: provides the capability to change room or reset the device (long press during boot).

### Pin Configuration
The components are connected to the NodeMCU as follows:

| Pin | Function                                  |
|-----|-------------------------------------------|
| D1  | SCL (shared by OLED, HDC1080, and DS3231) |
| D2  | SDA (shared by OLED, HDC1080, and DS3231) |
| D5  | Push Button                               |

## Setup and Installation
To set up the AirAnalyzer-Device software:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/davidepalladino/AirAnalyzer-Device.git
   ```
2. **Install PlatformIO**:
   Follow the instructions on the [PlatformIO website](https://platformio.org/install) to install the PlatformIO IDE.
3. **Open the Project**:
   Launch PlatformIO and open the cloned project directory.
4. **Configure the Environment**:
   Ensure that the `platformio.ini` file is set up correctly for the NodeMCU ESP8266 board.
5. **Upload the Firmware**:
   Connect your NodeMCU board to your computer and upload the firmware using PlatformIO.

## Usage
Once the device is set up:
- the OLED screen will display the current temperature and humidity readings;
- use the push button to change room.

## Related Resources
For more information, you can refer to the following resources:
- **[Air Analyzer v2.0.3](https://davidepalladino.github.io/2021/07/12/Air-Analyzer-v2-0-3/)**;
- **[Air Analyzer v3.0.0](https://davidepalladino.github.io/2022/02/13/Air-Analyzer-v3-0-0/)**.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments
Special thanks to the contributors and the open-source community for their support and resources.
