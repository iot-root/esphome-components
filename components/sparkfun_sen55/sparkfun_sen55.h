#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace sparkfun_sen55 {

class SEN55Component : public PollingComponent, public i2c::I2CDevice {
 public:
  // Default polling interval is 60 seconds
  SEN55Component() : PollingComponent(60000) {}

  // Setters for sensor pointers
  void set_pm_1_0_sensor(sensor::Sensor *pm) { pm_1_0_ = pm; }
  void set_pm_2_5_sensor(sensor::Sensor *pm) { pm_2_5_ = pm; }
  void set_pm_4_0_sensor(sensor::Sensor *pm) { pm_4_0_ = pm; }
  void set_pm_10_0_sensor(sensor::Sensor *pm) { pm_10_0_ = pm; }
  void set_voc_index_sensor(sensor::Sensor *voc) { voc_index_ = voc; }
  void set_nox_index_sensor(sensor::Sensor *nox) { nox_index_ = nox; }

  // Called by ESPHome when device is ready
  void setup() override;
  // Polled at the user-defined interval
  void update() override;

 protected:
  sensor::Sensor *pm_1_0_{nullptr};
  sensor::Sensor *pm_2_5_{nullptr};
  sensor::Sensor *pm_4_0_{nullptr};
  sensor::Sensor *pm_10_0_{nullptr};
  sensor::Sensor *voc_index_{nullptr};
  sensor::Sensor *nox_index_{nullptr};

  // Internal methods
  bool start_measurement_();
  bool read_sample_(float &pm1_0, float &pm2_5, float &pm4_0, float &pm10_0, float &voc_index, float &nox_index);
  bool execute_command_(uint16_t cmd, const uint16_t *args, size_t arg_len, uint16_t *resp, size_t resp_len);
};

}  // namespace sparkfun_sen55
}  // namespace esphome
