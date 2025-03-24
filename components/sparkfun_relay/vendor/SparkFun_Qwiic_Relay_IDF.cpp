#include "SparkFun_Qwiic_Relay_IDF.h"

SparkFun_Qwiic_Relay::SparkFun_Qwiic_Relay() : _address(0), _port(I2C_NUM_0) {}

bool SparkFun_Qwiic_Relay::begin(uint8_t address, i2c_port_t port) {
  _address = address;
  _port = port;
  // Send a dummy write (0-length) to check if the device is present.
  esp_err_t ret = i2c_master_write_to_device(_port, _address, nullptr, 0, pdMS_TO_TICKS(100));
  return (ret == ESP_OK);
}

esp_err_t SparkFun_Qwiic_Relay::writeCommand(uint8_t command) {
  return i2c_master_write_to_device(_port, _address, &command, 1, pdMS_TO_TICKS(1000));
}

void SparkFun_Qwiic_Relay::relayOn() {
  writeCommand(RELAY_ON_CMD);
}

void SparkFun_Qwiic_Relay::relayOff() {
  writeCommand(RELAY_OFF_CMD);
}
