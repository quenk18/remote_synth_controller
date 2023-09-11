#include <esp_log.h>
#include <inttypes.h>
#include <stdio.h>

#include "Adafruit_NeoPixel.h"
#include "Arduino.h"

static const char *TAG = "Test RGB LED Driver";
constexpr uint8_t RGB_LED_DIN = 10;
constexpr uint8_t LED_NB = 1;

extern "C" {
void app_main(void) {
        Adafruit_NeoPixel pixel(1, RGB_LED_DIN, NEO_GRB + NEO_KHZ800);
        while (1) {
                uint8_t pixel_no = 0;
                ESP_LOGI(TAG, "Setting RED color!!");
                pixel.setPixelColor(pixel_no, 255, 0, 0);
                pixel.show();
                delay(1000);
                ESP_LOGI(TAG, "Setting GREEN color!!");
                pixel.setPixelColor(pixel_no, 0, 255, 0);
                pixel.show();
                delay(1000);
                ESP_LOGI(TAG, "Setting BLUE color!!");
                pixel.setPixelColor(pixel_no, 0, 0, 255);
                pixel.show();
                delay(1000);
        }
}
}
