#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#define RED_GPIO 13
#define GREEN_GPIO 12

void redLED(void *pvParameter);
void greenLED(void *pvParameter);
void blinkLED(int pin, int hz);

 void app_main()
{
    xTaskCreate(&redLED, "redLED", 1024, NULL, 5, NULL);
    xTaskCreate(&greenLED, "greenLED", 1024, NULL, 5, NULL);
}
 
void redLED(void *pvParameter)
{
    blinkLED(RED_GPIO, 500);
}
 
void greenLED(void *pvParameter)
{
    blinkLED(GREEN_GPIO, 2500);
}

void blinkLED(int pin, int hz){
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    while(1) {
        gpio_set_level(pin, 0);
        vTaskDelay(hz / portTICK_RATE_MS);
        gpio_set_level(pin, 1);
        vTaskDelay(hz / portTICK_RATE_MS);
    }
}