#include "sparkfun_relay.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sparkfun_relay {

// Global instance of the SparkFun Qwiic Relay.
static SparkFun_QwiicRelay relay;

void SparkFunRelaySwitch::setup() {
  // Initialize the relay using the default I2C address 0x18.
  if (!relay.begin(0x18)) {
    ESP_LOGE("sparkfun_relay", "Failed to initialize SparkFun Qwiic Relay at address 0x18");
  } else {
    ESP_LOGI("sparkfun_relay", "SparkFun Qwiic Relay initialized successfully");
  }
}

void SparkFunRelaySwitch::write_state(bool state) {
  if (state) {
    relay.relayOn();
    ESP_LOGD("sparkfun_relay", "Relay turned ON");
  } else {
    relay.relayOff();
    ESP_LOGD("sparkfun_relay", "Relay turned OFF");
  }
  publish_state(state);
}

}  // namespace sparkfun_relay
}  // namespace esphome
