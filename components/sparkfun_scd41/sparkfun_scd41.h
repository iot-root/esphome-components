#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace sparkfun_scd41 {

// Note the class name here: SCD41Component
class SCD41Component : public PollingComponent, public i2c::I2CDevice {
 public:
  // Setters for each sensor
  void set_co2_sensor(sensor::Sensor *co2) { this->co2_ = co2; }
  void set_temperature_sensor(sensor::Sensor *temp) { this->temperature_ = temp; }
  void set_humidity_sensor(sensor::Sensor *rh) { this->humidity_ = rh; }

  void setup() override;
  void update() override;

 protected:
  sensor::Sensor *co2_{nullptr};
  sensor::Sensor *temperature_{nullptr};
  sensor::Sensor *humidity_{nullptr};

  bool send_command_(uint16_t cmd);
  bool read_data_(uint8_t *data, size_t len);
  uint8_t compute_crc8_(const uint8_t *data, size_t len);
};

}  // namespace sparkfun_scd41
}  // namespace esphome
