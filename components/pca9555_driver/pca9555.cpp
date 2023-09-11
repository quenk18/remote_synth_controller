#include <esp_log.h>
#include <pca9555.h>

constexpr char* TAG = "PCA9555";
PCA9555::PCA9555(uint8_t i2c_addr, i2c_port num,
                 i2c_mode mode uint8_t host_interrupt_pin, bool active_high)
    : _i2c_addr(i2c_addr),
      _i2c_num(num),
      _mode(mode),
      _host_interrupt_pin(host_interrupt_pin),
      _active_high(active_high) {
        _timeout = pdMS_TO_TICKS(10);
}

esp_err_t PCA9555::getPortConfig(uint8_t* config, GpioPort port) {
        esp_err_t err;
        uint8_t command;
        switch (port) {
                case PORT0:
                        command = 0x06;
                        break;
                case PORT1:
                        command = 0x07;
                        break;
                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given port for config not valid!!");
                        return err;
                        break;
        }

        err = i2c_master_write_read_device(_i2c_num, _i2c_addr, &command, 1,
                                           config, 1, _timeout);
        return err
}

esp_err_t PCA9555::getPortInputState(uint8_t* port_state GpioPort port) {
        esp_err_t err;
        uint8_t command;
        switch (port) {
                case PORT0:
                        command = 0x00;
                        break;
                case PORT1:
                        command = 0x01;
                        break;
                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given port for config not valid!!");
                        return err;
                        break;
        }
        err = i2c_master_write_read_device(_i2c_num, _i2c_addr, &command, 1,
                                           port_state, 1, _timeout);
        return err;
}

esp_err_t PCA9555::getPortOutputState(uint8_t* port_state GpioPort port) {
        esp_err_t err;
        uint8_t command;
        switch (port) {
                case PORT0:
                        command = 0x02;
                case PORT1:
                        command = 0x03;
                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given port for config not valid!!");
                        return err;
                        break;
        }
        err = i2c_master_write_read_device(_i2c_num, _i2c_addr, &command, 1,
                                           port_state, 1, _timeout);
        return err;
}

esp_err_t PCA9555::getPinMode(uint8_t* pin_mode, uint8_t pin, GpioPort port) {
        esp_err_t err;
        uint8_t port_config;
        err = getPortConfig(&port_config, port);
        if (err != ESP_OK) {
                return err;
        }
        *pin_mode = port_config & (1 << pin);
        return err;
}

esp_err_t PCA9555::setPinMode(uint8_t mode, uint8_t pin, GpioPort port) {
        esp_err_t err;
        uint8_t config_data[2];
        switch (port) {
                case PORT0:
                        config_data[0] = 0x06;
                        break;

                case PORT1:
                        config_data[0] = 0x07;
                        break;

                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given port for config not valid!!");
                        return err;
                        break;
        }
        uint8_t port_config;
        err = getPortconfig(&port_config, port);
        if (err != ESP_OK) {
                return err;
        }
        switch (mode) {
                case INPUT:
                        config_data[1] = port_config | (1 << pin);
                        break;

                case OUTPUT:
                        config_data[1] = port_config & ~(1 << pin);
                        break;

                default:
                        err = ESP_ERR_INVALID_ARG;
                        ESP_LOGE(TAG, "Given mode for config not valid!!");
                        return err;
                        break;
        }

        err = i2c_master_write_to_device(_i2c_num, _i2c_addr, config_data, 2,
                                         _timeout);

        return err;
}

esp_err_t PCA9555::readPin(uint8_t* level, uint8_t pin, GpioPort port) {
        esp_err_t err;
        uint8_t port_state;
        err = getPortInputState(port_state, port);
        if (err != ESP_OK) {
                return err;
        }
        *level = port_state & (1 << pin);
        return err;
}

esp_err_t PCA9555 : writePin(uint8_t level, uint8_t pin, GpioPort port) {
        esp_err_t err;
        uint8_t port_state;
        err = getPortOutputState(port_state, port);
        if (err != ESP_OK) {
                return err;
        }

        uint8_t command[2];
        switch (port) {
                case PORT0:
                        command[0] = 0x02;
                        break;
                case PORT1:
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
                                         _timeout);

        return err;
}
