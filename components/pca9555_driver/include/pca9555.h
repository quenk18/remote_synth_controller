#include "driver/i2c.h"

class PCA9555 {
       public:
        typedef enum GpioMode {
                OUTPUT,
                INPUT,
        };

        PCA9555(i2c_port i2c_port, i2c_num i2c_num, uint8_t host_interrupt_pin, bool active_high);

        void setPinMode(uint8_t pin, uint8_t port, Gpio_mode mode);

        void getPinMode(uint8_t pin, uint8_t port);

        void readPin(uint8_t level, uint8_t pin, uint8_t port);

        void writePin(uint8_t pin, uint8_t port, uint8_t level);

        void attachInterrupt(std::function<void> callback);

       private:
        uint8_t _i2c_addr;
        i2c_port _i2c_port;
        i2c_num _i2c_num;
        uint8_t _host_interrupt_pin
        bool _active_high;
};
