#include "driver/i2c.h"

class PCA9555 {
       public:
        enum class GpioPort {
                PORT0,
                PORT1,
        };

        PCA9555(uint8_t i2c_addr, i2c_port_t num, i2c_mode_t mode,
                uint8_t host_interrupt_pin, bool active_high);

        esp_err_t setPinMode(uint8_t mode, uint8_t pin, GpioPort port);

        esp_err_t getPinMode(uint8_t *pin_mode, uint8_t pin, GpioPort port);

        esp_err_t readPin(uint8_t *level, uint8_t pin, GpioPort port);

        esp_err_t writePin(uint8_t level, uint8_t pin, GpioPort port);

        //void attachInterrupt(std::function<void> callback);

       private:
        esp_err_t getPortConfig(uint8_t *config, GpioPort port);
        esp_err_t getPortInputState(uint8_t *port_state, GpioPort port);
        esp_err_t getPortOutputState(uint8_t *port_state, GpioPort port);

        uint8_t _i2c_addr;
        i2c_port_t _i2c_num;
        i2c_mode_t _mode;
        uint8_t _host_interrupt_pin;
        bool _active_high;
        TickType_t _timeout;
};
