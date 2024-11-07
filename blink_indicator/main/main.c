#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

#define BLINK_GPIO 48
#define MAX_BRIGHTNESS 255
#define COLOR_STEP 5 
static led_strip_handle_t led_strip;

static void set_led_color(uint32_t red, uint32_t green, uint32_t blue) {
    led_strip_set_pixel(led_strip, 0, red, green, blue);
    led_strip_refresh(led_strip);
}

static void configure_led(void) {
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1,
    };

    led_strip_spi_config_t spi_config = {
        .spi_bus = SPI2_HOST,
        .flags.with_dma = true,
    };

    if (led_strip_new_spi_device(&strip_config, &spi_config, &led_strip) != ESP_OK) {
        return;
    }
    led_strip_clear(led_strip);
}

void app_main(void) {
    uint32_t red = MAX_BRIGHTNESS, green = 0, blue = 0;
    int transition_phase = 0;

    configure_led();

    while (1) {
        switch (transition_phase) {
            case 0:
                red -= COLOR_STEP;
                green += COLOR_STEP;
                if (green >= MAX_BRIGHTNESS) {
                    green = MAX_BRIGHTNESS;
                    red = 0;
                    transition_phase = 1;
                }
                break;

            case 1:
                green -= COLOR_STEP;
                blue += COLOR_STEP;
                if (blue >= MAX_BRIGHTNESS) {
                    blue = MAX_BRIGHTNESS;
                    green = 0;
                    transition_phase = 2;
                }
                break;

            case 2:
                blue -= COLOR_STEP;
                red += COLOR_STEP;
                if (red >= MAX_BRIGHTNESS) {
                    red = MAX_BRIGHTNESS;
                    blue = 0;
                    transition_phase = 0;
                }
                break;
        }

        set_led_color(red, green, blue);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
