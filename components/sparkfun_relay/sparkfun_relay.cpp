#include "sparkfun_relay.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sparkfun_relay {

static const char *TAG = "sparkfun_relay";

void SparkFunRelaySwitch::setup() {
  // The I2C bus is configured in ESPHome and available via this->parent, but for
  // our simple component we assume only one I2C bus is used.
  // ESPHome uses its I2CDevice abstraction for I2C communication.
  // Here, we assume that the ESPHome I2C bus is already set up and we can simply use it.
  // For this example, we use I2C_NUM_0 as the bus. In a more advanced component,
  // you might retrieve the bus pointer from a parent I2CDevice.
  if (!relay_.begin(0x18, I2C_NUM_0)) {
    ESP_LOGE(TAG, "Failed to initialize SparkFun Qwiic Relay on ESP-IDF");
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
