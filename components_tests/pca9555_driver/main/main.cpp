#include <esp_log.h>
#include <inttypes.h>
#include <pca9555.h>
#include <stdio.h>

#include "driver/i2c.h"

static const char *TAG = "Test PCA9555";

constexpr uint8_t PCA9555_I2C_ADDR = 0x20;
constexpr uint8_t ESP32_SDA_PIN = 4;
constexpr uint8_t ESP32_SCL_PIN = 5;
constexpr uint32_t ESP32_I2C_CLK_SPEED = 100000;

constexpr uint8_t INTERRUPT_PIN = 6;
extern "C" {
void app_main(void) {
        esp_err_t err;

        i2c_port_t i2c_port = I2C_NUM_0;
        i2c_config_t i2c_config;
        i2c_config.mode = I2C_MODE_MASTER;
        i2c_config.sda_io_num = ESP32_SDA_PIN;
        i2c_config.scl_io_num = ESP32_SCL_PIN;
        i2c_config.sda_pullup_en = GPIO_PULLUP_DISABLE;
        i2c_config.scl_pullup_en = GPIO_PULLUP_DISABLE;
        i2c_config.master.clk_speed = ESP32_I2C_CLK_SPEED;
        i2c_config.clk_flags = 0;

        err = i2c_param_config(i2c_port, &i2c_config);
        if (err != ESP_OK) {
                ESP_LOGE(TAG, "Failed to configure i2c parameters!!");
                ESP_LOGE(TAG, "%s", esp_err_to_name(err));
        }
        err = i2c_driver_install(i2c_port, i2c_config.mode, 0, 0, 0);
        if (err != ESP_OK) {
                ESP_LOGE(TAG, "Failed to install i2c driver!!");
                ESP_LOGE(TAG, "%s", esp_err_to_name(err));
        }

        PCA9555 GpioExpander(PCA9555_I2C_ADDR, I2C_NUM_0, I2C_MODE_MASTER,
                             INTERRUPT_PIN, true);

        for (int i = 0; i < 8; i++) {
                err = GpioExpander.setPinMode(1, i, PCA9555::GpioPort::PORT1);
                if (err != ESP_OK) {
                        ESP_LOGE(TAG,
                                 "Failed to set GPIO Mode of GPIO Expander!!");
                        ESP_LOGE(TAG, "%s", esp_err_to_name(err));
                }
        }

        while (1) {
                for (int i = 0; i < 8; i++) {
                        uint8_t level;
                        GpioExpander.readPin(&level, i,
                                             PCA9555::GpioPort::PORT1);
                        ESP_LOGI(TAG, "Level of %d: %d", i, level);
                }
                vTaskDelay(pdMS_TO_TICKS(1000));
        }
}
}
