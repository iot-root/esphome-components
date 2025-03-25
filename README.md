**Abstract:**  
This README documents three custom ESPHome external components: sparkfun_scd41, sparkfun_sen55, and sparkfun_relay. They allow seamless integration of SparkFun’s SCD41 CO₂ sensor, SEN55 environment sensor, and SparkFun’s Qwiic 4-channel relay into ESPHome. Each folder contains YAML metadata, C++ code, and Python integration files conforming to ESPHome’s external component standards. They are compatible with Arduino and ESP-IDF frameworks, making them ideal for various ESP32 or ESP8266 boards.

# ESPHome Components for SparkFun Sensors and Relay

This repository provides three separate custom ESPHome components:
1. **sparkfun_scd41** – For the SparkFun SCD41 sensor (CO₂, temperature, humidity).  
2. **sparkfun_sen55** – For the SparkFun SEN55 sensor (particulate matter, VOC index, NOx index).  
3. **sparkfun_relay** – For the SparkFun Qwiic single relay board.

Each component can be referenced in your ESPHome configuration to integrate the hardware seamlessly with Home Assistant or other compatible systems. All components are self-contained, avoiding reliance on external Arduino libraries, and support both the Arduino and ESP-IDF frameworks.

---

## Repository Structure

```
esphome-components/
  sparkfun_scd41/
    component.yaml
    sparkfun_scd41.h
    sparkfun_scd41.cpp
    sensor.py
  sparkfun_sen55/
    component.yaml
    sparkfun_sen55.h
    sparkfun_sen55.cpp
    sensor.py
  sparkfun_relay/
    component.yaml
    sparkfun_relay.h
    sparkfun_relay.cpp
    switch.py
```

### sparkfun_scd41
- Measures **CO₂**, **temperature**, and **humidity** using the Sensirion SCD41 sensor.  
- Default I²C address: `0x62`.

### sparkfun_sen55
- Provides **PM1.0**, **PM2.5**, **PM4.0**, **PM10.0**, **VOC index**, and **NOx index** from the SEN55 sensor module.  
- Default I²C address: `0x69`.

### sparkfun_relay
- Controls the **SparkFun Qwiic 4-channel relay**.  
- Accesses relays over I²C for switching purposes.  
- Default I²C address: `0x25` (depending on relay board configuration).

---

## How to Use

1. **Add the Repository in Your ESPHome Config**  
   ```yaml
   external_components:
     - source:
         type: git
         url: https://github.com/iot-root/esphome-components
         ref: main
       components: ["sparkfun_scd41", "sparkfun_sen55", "sparkfun_relay"]
   ```

2. **Enable the I²C Bus**  
   ```yaml
   i2c:
     sda: GPIOXX
     scl: GPIOXX
   ```

3. **Configure Each Component**

   **SCD41 Example**:
   ```yaml
   sensor:
     - platform: sparkfun_scd41
       co2:
         name: "SCD41 CO2"
       temperature:
         name: "SCD41 Temperature"
       humidity:
         name: "SCD41 Humidity"
       update_interval: 60s
   ```

   **SEN55 Example**:
   ```yaml
   sensor:
     - platform: sparkfun_sen55
       pm_1_0:
         name: "SEN55 PM1.0"
       pm_2_5:
         name: "SEN55 PM2.5"
       pm_4_0:
         name: "SEN55 PM4.0"
       pm_10_0:
         name: "SEN55 PM10.0"
       voc_index:
         name: "SEN55 VOC Index"
       nox_index:
         name: "SEN55 NOx Index"
       update_interval: 60s
   ```

   **Relay Example**:
   ```yaml
   switch:
     - platform: sparkfun_relay
       channel: 1
       name: "Relay Channel 1"

     - platform: sparkfun_relay
       channel: 2
       name: "Relay Channel 2"
   ```

4. **Compile and Upload**  
   - Use the ESPHome dashboard or the CLI to compile and upload the firmware to your ESP device.

---

## Contributing

We welcome contributions!  
1. **Fork** this repository.  
2. **Create** a feature branch.  
3. **Implement** your changes in the relevant component folder (C++ or Python files).  
4. **Open** a pull request to merge your branch into `main`.

