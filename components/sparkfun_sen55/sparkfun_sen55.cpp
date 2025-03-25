#include "sparkfun_sen55.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sparkfun_sen55 {

static const char *const TAG = "sparkfun_sen55";

void SEN55Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SEN55 sensor...");

  // Attempt to start measurement
  if (!start_measurement_()) {
    ESP_LOGW(TAG, "Failed to start measurement on SEN55");
  }
}

void SEN55Component::update() {
  float pm1_0, pm2_5, pm4_0, pm10_0, voc_idx, nox_idx;
  if (!read_sample_(pm1_0, pm2_5, pm4_0, pm10_0, voc_idx, nox_idx)) {
    ESP_LOGW(TAG, "Failed to read data from SEN55");
    return;
  }

  if (pm_1_0_ != nullptr) pm_1_0_->publish_state(pm1_0);
  if (pm_2_5_ != nullptr) pm_2_5_->publish_state(pm2_5);
  if (pm_4_0_ != nullptr) pm_4_0_->publish_state(pm4_0);
  if (pm_10_0_ != nullptr) pm_10_0_->publish_state(pm10_0);
  if (voc_index_ != nullptr) voc_index_->publish_state(voc_idx);
  if (nox_index_ != nullptr) nox_index_->publish_state(nox_idx);
}

bool SEN55Component::start_measurement_() {
  // 0x0021 = start measurement
  return execute_command_(0x0021, nullptr, 0, nullptr, 0);
}

bool SEN55Component::read_sample_(
  float &pm1_0, float &pm2_5, float &pm4_0, float &pm10_0, float &voc_index, float &nox_index
) {
  // 0x03C4 = read measured values
  // We expect multiple 16-bit words
  uint16_t resp[12];
  if (!execute_command_(0x03C4, nullptr, 0, resp, 12)) {
    return false;
  }

  // Each measurement is a 16-bit integer that needs scaling
  pm1_0 = static_cast<float>(resp[0]) / 10.0f;
  pm2_5 = static_cast<float>(resp[1]) / 10.0f;
  pm4_0 = static_cast<float>(resp[2]) / 10.0f;
  pm10_0 = static_cast<float>(resp[3]) / 10.0f;
  voc_index = static_cast<float>(resp[4]) / 10.0f;
  nox_index = static_cast<float>(resp[5]) / 10.0f;

  return true;
}

bool SEN55Component::execute_command_(
  uint16_t cmd, const uint16_t *args, size_t arg_len, uint16_t *resp, size_t resp_len
) {
  // Write command + arguments with CRC
  const size_t arg_buf_len = arg_len * 3;
  uint8_t tx_buf[2 + arg_buf_len];
  tx_buf[0] = static_cast<uint8_t>(cmd >> 8);
  tx_buf[1] = static_cast<uint8_t>(cmd & 0xFF);

  // Fill argument buffer
  size_t offset = 2;
  for (size_t i = 0; i < arg_len; i++) {
    uint16_t val = args[i];
    tx_buf[offset] = static_cast<uint8_t>(val >> 8);
    tx_buf[offset + 1] = static_cast<uint8_t>(val & 0xFF);

    // Compute 8-bit CRC
    uint8_t crc = 0xFF;
    uint8_t data[2] = {tx_buf[offset], tx_buf[offset + 1]};
    for (uint8_t b = 0; b < 2; b++) {
      crc ^= data[b];
      for (uint8_t bit = 0; bit < 8; bit++) {
        if (crc & 0x80) {
          crc = (crc << 1) ^ 0x31;
        } else {
          crc <<= 1;
        }
      }
    }
    tx_buf[offset + 2] = crc;
    offset += 3;
  }

  if (!this->write(tx_buf, 2 + arg_buf_len, true)) {
    return false;
  }
  delay(50);  // Give sensor time to process

  // If no response requested
  if (resp_len == 0) {
    return true;
  }

  // Read response
  uint8_t rx_buf[resp_len * 3];
  if (!this->read(rx_buf, resp_len * 3)) {
    return false;
  }

  // Parse each 16-bit word with CRC check
  for (size_t i = 0; i < resp_len; i++) {
    uint8_t d0 = rx_buf[i * 3];
    uint8_t d1 = rx_buf[i * 3 + 1];
    uint8_t c = rx_buf[i * 3 + 2];

    // Recompute CRC
    uint8_t crc = 0xFF;
    uint8_t data[2] = {d0, d1};
    for (uint8_t b = 0; b < 2; b++) {
      crc ^= data[b];
      for (uint8_t bit = 0; bit < 8; bit++) {
        if (crc & 0x80) {
          crc = (crc << 1) ^ 0x31;
        } else {
          crc <<= 1;
        }
      }
    }

    if (crc != c) {
      ESP_LOGW(TAG, "CRC mismatch in SEN55 response at index %zu", i);
      return false;
    }

    resp[i] = static_cast<uint16_t>(d0 << 8 | d1);
  }
  return true;
}

}  // namespace sparkfun_sen55
}  // namespace esphome
