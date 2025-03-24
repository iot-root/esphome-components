#include "sparkfun_relay.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sparkfun_relay {

static const char *TAG = "sparkfun_relay";

// Implementation of the vendor code.
SparkFun_Qwiic_Relay::SparkFun_Qwiic_Relay() : _address(0), _port(I2C_NUM_0) {}

bool SparkFun_Qwiic_Relay::begin(uint8_t address, i2c_port_t port) {
  _address = address;
  _port = port;
  // Attempt a dummy write to verify the device is present.
  esp_err_t ret = i2c_master_write_to_device(_port, _address, nullptr, 0, pdMS_TO_TICKS(100));
  return (ret == ESP_OK);
}

esp_err_t SparkFun_Qwiic_Relay::writeCommand(uint8_t command) {
  return i2c_master_write_to_device(_port, _address, &command, 1, pdMS_TO_TICKS(1000));
}

void SparkFun_Qwiic_Relay::relayOn() {
  writeCommand(0x01);  // Relay ON command.
}

void SparkFun_Qwiic_Relay::relayOff() {
  writeCommand(0x00);  // Relay OFF command.
}

// Implementation of the external component.
void SparkFunRelaySwitch::setup() {
  if (!relay_.begin(0x18, I2C_NUM_0)) {
    ESP_LOGE(TAG, "Failed to initialize SparkFun Qwiic Relay");
  } else {
    ESP_LOGI(TAG, "SparkFun Qwiic Relay initialized successfully");
  }
}

void SparkFunRelaySwitch::write_state(bool state) {
  if (state) {
    relay_.relayOn();
    ESP_LOGD(TAG, "Relay turned ON");
  } else {
    relay_.relayOff();
    ESP_LOGD(TAG, "Relay turned OFF");
  }
  publish_state(state);
}

}  // namespace sparkfun_relay
}  // namespace esphome
