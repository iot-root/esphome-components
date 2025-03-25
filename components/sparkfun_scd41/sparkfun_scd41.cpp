#include "sparkfun_scd41.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"  // Allows esphome::delay() under ESP-IDF

namespace esphome {
namespace sparkfun_scd41 {

static const char *const TAG = "sparkfun_scd41";

void SCD41Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SCD41 sensor...");

  // Send Stop Measurement command first (in case it's already running)
  send_command_(0x3F86);

  // Start periodic measurement
  if (!send_command_(0x21B1)) {
    ESP_LOGW(TAG, "Failed to start periodic measurement on SCD41");
  }
}

void SCD41Component::update() {
  uint16_t co2_raw;
  float temp = 0.0f;
  float hum = 0.0f;

  if (!read_measurement_(co2_raw, temp, hum)) {
    ESP_LOGW(TAG, "Failed to read measurement from SCD41");
    return;
  }

  float co2_val = static_cast<float>(co2_raw);

  if (this->co2_ != nullptr) {
    this->co2_->publish_state(co2_val);
  }
  if (this->temperature_ != nullptr) {
    this->temperature_->publish_state(temp);
  }
  if (this->humidity_ != nullptr) {
    this->humidity_->publish_state(hum);
  }
}

bool SCD41Component::send_command_(uint16_t cmd, uint16_t argument, bool has_argument) {
  // Construct the command buffer
  // Some commands accept a 16-bit argument + CRC
  uint8_t buf[5];
  buf[0] = static_cast<uint8_t>(cmd >> 8);
  buf[1] = static_cast<uint8_t>(cmd & 0xFF);

  int len = 2;
  if (has_argument) {
    buf[2] = static_cast<uint8_t>(argument >> 8);
    buf[3] = static_cast<uint8_t>(argument & 0xFF);
    buf[4] = compute_crc8_(&buf[2], 2);
    len = 5;
  }

  return this->write(buf, len, true);
}

bool SCD41Component::read_measurement_(uint16_t &co2, float &temp, float &hum) {
  // Send the read measurement command (0xEC05)
  if (!send_command_(0xEC05)) {
    return false;
  }

  // Use esphome::delay(1) instead of delay(1) for ESP-IDF compatibility
  esphome::delay(1);  // Brief wait for data to become ready

  uint8_t data[9];
  if (!this->read(data, 9)) {
    return false;
  }

  // Parse CO2
  if (compute_crc8_(data, 2) != data[2]) {
    return false;
  }
  co2 = static_cast<uint16_t>((data[0] << 8) | data[1]);

  // Parse Temperature
  if (compute_crc8_(&data[3], 2) != data[5]) {
    return false;
  }
  uint16_t raw_temp = static_cast<uint16_t>((data[3] << 8) | data[4]);
  temp = -45.0f + 175.0f * (static_cast<float>(raw_temp) / 65535.0f);

  // Parse Humidity
  if (compute_crc8_(&data[6], 2) != data[8]) {
    return false;
  }
  uint16_t raw_hum = static_cast<uint16_t>((data[6] << 8) | data[7]);
  hum = 100.0f * (static_cast<float>(raw_hum) / 65535.0f);

  return true;
}

uint8_t SCD41Component::compute_crc8_(uint8_t *data, uint8_t len) {
  // SCD41 uses Sensirion's CRC-8 with polynomial 0x31
  uint8_t crc = 0xFF;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t b = 0; b < 8; b++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

}  // namespace sparkfun_scd41
}  // namespace esphome
