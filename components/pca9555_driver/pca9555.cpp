#include "pca9555.h"

#include <esp_log.h>

constexpr char* TAG = "PCA9555";
PCA9555::PCA9555(uint8_t i2c_addr, i2c_port_t num, i2c_mode_t mode,
                 uint8_t host_interrupt_pin, bool active_high)
    : _i2c_addr(i2c_addr),
      _i2c_num(num),
      _mode(mode),
      _host_interrupt_pin(host_interrupt_pin),
      _active_high(active_high) {}

esp_err_t PCA9555::getPortConfig(uint8_t* config, GpioPort port) {
        esp_err_t err;
        uint8_t command;
        switch (port) {
                case GpioPort::PORT0:
                        command = 0x06;
                        break;
                case GpioPort::PORT1:
                        command = 0x07;
                        break;
                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given port for config not valid!!");
                        return err;
                        break;
        }

        err =
            i2c_master_write_read_device(_i2c_num, _i2c_addr, &command, 1,
                                         config, 1, 1000 / portTICK_PERIOD_MS);
        return err;
}

esp_err_t PCA9555::getPortInputState(uint8_t* port_state, GpioPort port) {
        esp_err_t err;
        uint8_t command;
        switch (port) {
                case GpioPort::PORT0:
                        command = 0x00;
                        break;
                case GpioPort::PORT1:
                        command = 0x01;
                        break;
                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given port for config not valid!!");
                        return err;
                        break;
        }
        err = i2c_master_write_read_device(_i2c_num, _i2c_addr, &command, 1,
                                           port_state, 1,
                                           1000 / portTICK_PERIOD_MS);
        return err;
}

esp_err_t PCA9555::getPortOutputState(uint8_t* port_state, GpioPort port) {
        esp_err_t err;
        uint8_t command;
        switch (port) {
                case GpioPort::PORT0:
                        command = 0x02;
                        break;
                case GpioPort::PORT1:
                        command = 0x03;
                        break;
                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given port for config not valid!!");
                        return err;
                        break;
        }
        err = i2c_master_write_read_device(_i2c_num, _i2c_addr, &command, 1,
                                           port_state, 1,
                                           1000 / portTICK_PERIOD_MS);
        return err;
}

esp_err_t PCA9555::getPinMode(uint8_t* pin_mode, uint8_t pin, GpioPort port) {
        esp_err_t err;
        uint8_t port_config;
        err = getPortConfig(&port_config, port);
        if (err != ESP_OK) {
                return err;
        }
        if ((port_config >> pin) & 1) {
                *pin_mode = 1;
        } else {
                *pin_mode = 0;
        }
        return err;
}

esp_err_t PCA9555::setPinMode(uint8_t mode, uint8_t pin, GpioPort port) {
        esp_err_t err;
        uint8_t config_data[2];
        switch (port) {
                case GpioPort::PORT0:
                        config_data[0] = 0x06;
                        break;

                case GpioPort::PORT1:
                        config_data[0] = 0x07;
                        break;

                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given port for config not valid!!");
                        return err;
                        break;
        }
        uint8_t port_config;
        err = getPortConfig(&port_config, port);
        if (err != ESP_OK) {
                ESP_LOGE(TAG, "Failed to get port config!!");
                return err;
        }
        switch (mode) {
                case 1:  // INPUT
                        config_data[1] = port_config | (1 << pin);
                        break;

                case 0:  // OUTPUT
                        config_data[1] = port_config & ~(1 << pin);
                        break;

                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given mode for config not valid!!");
                        return err;
                        break;
        }

        err = i2c_master_write_to_device(_i2c_num, _i2c_addr, config_data, 2,
                                         1000 / portTICK_PERIOD_MS);

        return err;
}

esp_err_t PCA9555::readPin(uint8_t* level, uint8_t pin, GpioPort port) {
        esp_err_t err;
        uint8_t port_state;
        err = getPortInputState(&port_state, port);
        if (err != ESP_OK) {
                return err;
        }
        if (port_state & (1 << pin)) {
                *level = 1;
        } else {
                *level = 0;
        }
        return err;
}

esp_err_t PCA9555::writePin(uint8_t level, uint8_t pin, GpioPort port) {
        esp_err_t err;
        uint8_t port_state;
        err = getPortOutputState(&port_state, port);
        if (err != ESP_OK) {
                return err;
        }

        uint8_t command[2];
        switch (port) {
                case GpioPort::PORT0:
                        command[0] = 0x02;
                        break;
                case GpioPort::PORT1:
                        command[0] = 0x03;
                        break;
                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given port for config not valid!!");
                        return err;
                        break;
        }
        switch (level) {
                case 1:
                        command[1] = port_state | (1 << pin);
                        break;
                case 0:

                        command[1] = port_state & ~(1 << pin);
                        break;
                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given level for config not valid!!");
                        return err;
                        break;
        }
        err = i2c_master_write_to_device(_i2c_num, _i2c_addr, command, 2,
                                         1000 / portTICK_PERIOD_MS);

        return err;
}
