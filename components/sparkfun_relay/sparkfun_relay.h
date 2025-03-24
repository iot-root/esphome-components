#pragma once
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include <SparkFun_Qwiic_Relay.h>

namespace esphome {
namespace sparkfun_relay {

/**
 * @brief A switch component to control a SparkFun Qwiic Relay.
 *
 * This component uses the SparkFun Qwiic Relay Arduino Library to manage the relay's state.
 */
class SparkFunRelaySwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void write_state(bool state) override;
};

}  // namespace sparkfun_relay
}  // namespace esphome
