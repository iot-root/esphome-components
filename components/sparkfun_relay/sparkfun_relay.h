#pragma once
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "driver/i2c.h"
#include "esp_err.h"

namespace esphome {
namespace sparkfun_relay {

// Ported vendor code integrated into the header.
class SparkFun_Qwiic_Relay {
 public:
  SparkFun_Qwiic_Relay();
  bool begin(uint8_t address, i2c_port_t port);
  void relayOn();
  void relayOff();
  
 private:
  uint8_t _address;
  i2c_port_t _port;
  esp_err_t writeCommand(uint8_t command);
};

// External component that uses the relay.
class SparkFunRelaySwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void write_state(bool state) override;

 protected:
  SparkFun_Qwiic_Relay relay_;
};

}  // namespace sparkfun_relay
}  // namespace esphome
