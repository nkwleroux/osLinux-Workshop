#include <stdio.h>
#include "sdkconfig.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void nvs_flash_init();

void print_task(void *pvParameter)
{
    int i = (int) pvParameter;
    int j = 21 - i;
    printf("Task %d\n",j);
    vTaskDelete( 0 );
} 
 
void app_main()
{
    nvs_flash_init();
    for (int i = 20; i > 0; i--)
    {
        xTaskCreate(&print_task, "print_task", 2048, (void*)i, i,NULL);
        TickType_t xLastExecutionTime = xTaskGetTickCount();
        vTaskDelayUntil( &xLastExecutionTime, pdMS_TO_TICKS( 10 ) );
    }
}