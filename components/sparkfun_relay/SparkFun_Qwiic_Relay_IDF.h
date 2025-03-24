#ifndef SPARKFUN_QWIIC_RELAY_IDF_H
#define SPARKFUN_QWIIC_RELAY_IDF_H

#include "driver/i2c.h"
#include "esp_err.h"

// Command definitions (adjust these if needed per your relay documentation)
#define RELAY_ON_CMD  0x01
#define RELAY_OFF_CMD 0x00

class SparkFun_Qwiic_Relay {
 public:
  SparkFun_Qwiic_Relay();

  /**
   * @brief Initialize the relay with the given I2C address and port.
   *
   * @param address I2C address (default 0x18)
   * @param port I2C port number (use I2C_NUM_0 for our single bus case)
   * @return true if communication is successful, false otherwise.
   */
  bool begin(uint8_t address, i2c_port_t port);

  /**
   * @brief Turn the relay on.
   */
  void relayOn();

  /**
   * @brief Turn the relay off.
   */
  void relayOff();

 private:
  uint8_t _address;
  i2c_port_t _port;
  esp_err_t writeCommand(uint8_t command);
};

#endif // SPARKFUN_QWIIC_RELAY_IDF_H
