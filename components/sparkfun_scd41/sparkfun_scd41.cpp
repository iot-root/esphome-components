#include "sparkfun_scd41.h"
#include "esphome/core/log.h"
// For ESP-IDF delays, include FreeRTOS headers:
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace esphome {
namespace sparkfun_scd41 {

static const char *const TAG = "sparkfun_scd41";

// Class name must match the .h file: SCD41Component
void SCD41Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Sparkfun SCD41...");

  // Start periodic measurement (0x21B1)
  if (!this->send_command_(0x21B1)) {
    ESP_LOGE(TAG, "Could not send start measurement command");
    this->mark_failed();
  }
}

void SCD41Component::update() {
  // Send read-measurement command (0xEC05)
  if (!this->send_command_(0xEC05)) {
    ESP_LOGW(TAG, "Could not request SCD41 measurement");
    return;
  }
  // Wait 1 ms (FreeRTOS style)
  vTaskDelay(pdMS_TO_TICKS(1));

  // Read 9 bytes: 3 words of data + 3 CRC bytes
  uint8_t raw[9];
  if (!this->read_data_(raw, sizeof(raw))) {
    ESP_LOGW(TAG, "Failed to read measurement from SCD41");
    return;
  }

  // Validate CRC on each 2-byte word
  for (int i = 0; i < 3; i++) {
    uint8_t *chunk = &raw[i * 3];
    uint8_t actual_crc = chunk[2];
    uint8_t expected_crc = this->compute_crc8_(chunk, 2);
    if (actual_crc != expected_crc) {
      ESP_LOGW(TAG, "CRC mismatch on word %d: got 0x%02X expected 0x%02X", i, actual_crc, expected_crc);
      return;
    }
  }

  // Parse raw values
  uint16_t co2_raw = (uint16_t(raw[0]) << 8) | raw[1];
  uint16_t temp_raw = (uint16_t(raw[3]) << 8) | raw[4];
  uint16_t rh_raw   = (uint16_t(raw[6]) << 8) | raw[7];

  // Convert to standard units
  float co2_ppm = float(co2_raw); // 1:1
  float temperature_c = -45.0f + 175.0f * (float(temp_raw) / 65536.0f);
  float humidity_percent = 100.0f * (float(rh_raw) / 65536.0f);

  // Publish readings
  if (this->co2_ != nullptr)
    this->co2_->publish_state(co2_ppm);

  if (this->temperature_ != nullptr)
    this->temperature_->publish_state(temperature_c);

  if (this->humidity_ != nullptr)
    this->humidity_->publish_state(humidity_percent);
}

bool SCD41Component::send_command_(uint16_t cmd) {
  uint8_t buf[2] = {
    static_cast<uint8_t>(cmd >> 8),
    static_cast<uint8_t>(cmd & 0xFF),
  };
  return this->write(buf, 2);
}

bool SCD41Component::read_data_(uint8_t *data, size_t len) {
  return this->read(data, len);
}

uint8_t SCD41Component::compute_crc8_(const uint8_t *data, size_t len) {
  // Polynomial 0x31, init 0xFF (from Sensirion docs)
  uint8_t crc = 0xFF;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (int b = 0; b < 8; b++) {
      if (crc & 0x80)
        crc = (crc << 1) ^ 0x31;
      else
        crc <<= 1;
    }
  }
  return crc;
}

}  // namespace sparkfun_scd41
}  // namespace esphome
