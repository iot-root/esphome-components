#pragma once
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
// Use our ported IDF version from the vendor folder:
#include "esphome/components/sparkfun_relay/vendor/SparkFun_Qwiic_Relay_IDF.h"

namespace esphome {
namespace sparkfun_relay {

/** 
 * @brief ESPHome switch component for SparkFun Qwiic Relay using ESP-IDF.
 */
class SparkFunRelaySwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void write_state(bool state) override;

 protected:
  // Our ported relay object.
  SparkFun_Qwiic_Relay relay_;
};

}  // namespace sparkfun_relay
}  // namespace esphome
