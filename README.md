# SparkFun Relay ESPHome Component

This repository contains a custom ESPHome external component that integrates the SparkFun Qwiic Relay using a ported version of the SparkFun Qwiic Relay library for ESP-IDF. It is designed for ESP32-S3 devices running ESPHome with the ESP-IDF framework, allowing you to control a SparkFun Qwiic Relay (default I²C address 0x18) through ESPHome's switch interface.

## Features

- **ESP-IDF Port**: Uses native ESP-IDF I²C APIs for reliable relay control without the Arduino compatibility layer.
- **Integrated Code**: Vendor code is refactored and merged into the main component, simplifying the build process.
- **ESPHome Integration**: Easily integrates as an external component in your ESPHome YAML configuration.
- **Simple Configuration**: Automatically uses the configured I²C bus (default is I2C_NUM_0).

## Prerequisites

- **Hardware**: An ESP32-S3 board (e.g., Adafruit Feather ESP32-S3) with I²C connectivity.
- **Software**: ESPHome version 2025.3.1 or later configured to use the ESP-IDF framework.
- **I²C Setup**: A properly configured I²C bus in your ESPHome YAML file.

## Installation

### Via ESPHome External Components

Add the following to your ESPHome YAML configuration to include this component:

```yaml
external_components:
  - source: github://iot-root/sparkfun_relay
```

Ensure your I²C configuration is set up correctly:

```yaml
i2c:
  sda: GPIO7   # Replace with your SDA pin
  scl: GPIO6   # Replace with your SCL pin
  scan: True
```

Then, configure the switch that controls the relay:

```yaml
switch:
  - platform: sparkfun_relay
    name: "SparkFun Qwiic Relay"
    id: sparkfun_qwiic_relay
```

## How It Works

The component is implemented as an ESPHome external component that provides a switch interface to control the SparkFun Qwiic Relay. The relay functionality is ported to use ESP-IDF's native I²C functions, eliminating the dependency on Arduino libraries. During setup, the relay is initialized on the default I²C bus (I2C_NUM_0) and address (0x18). When the switch is toggled in Home Assistant, the component calls the corresponding `relayOn` or `relayOff` functions to control the hardware relay.

## Files Structure

```
components/sparkfun_relay/
├── __init__.py
├── manifest.json         # (Optional) For external component management.
├── sparkfun_relay.cpp    # Contains the component and relay implementation.
├── sparkfun_relay.h      # Header file with component definitions and ported vendor code.
├── switch.py             # Python file for ESPHome YAML registration.
```

## Contributing

Contributions are welcome! If you encounter issues or have suggestions for improvement, please open an issue or submit a pull request. Make sure to follow the [ESPHome contributing guidelines](https://esphome.io/guides/contributing.html) when making changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Troubleshooting

- **Undefined References**: Ensure that all source files are compiled by verifying the `manifest.json` or directory structure. If you encounter linking errors, try cleaning your build cache.
- **I²C Bus Issues**: Confirm that your I²C pins are correctly configured in your YAML and that no conflicts occur with other hardware interfaces.
- **PSRAM Warnings**: Some warnings related to PSRAM or strapping pins may appear on ESP32-S3 devices. Review the ESPHome FAQ for guidance on avoiding these warnings.

