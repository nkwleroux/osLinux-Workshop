/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void nvs_flash_init();
void priority_task(void *pvParameter);

    void app_main()
    {
        nvs_flash_init();
        xTaskCreate(&priority_task, "priority_task", 2048, NULL, 5, 0);
    }
        
    void priority_task(void *pvParameter)
    {
        printf("Task Priority set to 5\n");
        for (int i = 4; i > 0; i--)
        {
            for (int j = 0; j < 2; j++)
            {
                printf("Another 3 seconds!\n");
                printf("Current Priority is %d\n", i + 1);
                vTaskDelay(pdMS_TO_TICKS(3000));
            }
            printf("Task Priority set to %d\n", i);
            vTaskPrioritySet(0, i);
        }
        printf("Task deleted\n");
        vTaskDelete(0);
    }
