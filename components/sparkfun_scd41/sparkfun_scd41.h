#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace sparkfun_scd41 {

class SCD41Component : public PollingComponent, public i2c::I2CDevice {
 public:
  // Default polling interval is 60 seconds
  SCD41Component() : PollingComponent(60000) {}

  // Setters for sensor pointers
  void set_co2_sensor(sensor::Sensor *co2) { co2_ = co2; }
  void set_temperature_sensor(sensor::Sensor *temp) { temperature_ = temp; }
  void set_humidity_sensor(sensor::Sensor *humid) { humidity_ = humid; }

  // Called by framework after I²C bus is ready
  void setup() override;
  // Called periodically based on polling interval
  void update() override;

 protected:
  // Internal references to our sensor objects
  sensor::Sensor *co2_{nullptr};
  sensor::Sensor *temperature_{nullptr};
  sensor::Sensor *humidity_{nullptr};

  // Internal methods for communication
  bool send_command_(uint16_t cmd, uint16_t argument = 0, bool has_argument = false);
  bool read_measurement_(uint16_t &co2, float &temp, float &hum);
  uint8_t compute_crc8_(uint8_t *data, uint8_t len);
};

}  // namespace sparkfun_scd41
}  // namespace esphome
