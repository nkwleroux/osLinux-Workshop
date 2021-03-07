#include <stdio.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

SemaphoreHandle_t mutex;
char text[20] = "";

void reader_task(void *pvPar)
{
    while (1)
    {
        while (uxSemaphoreGetCount(mutex) == 0)
        {
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
        printf("Read %s\n", text);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void writer_task(void *pvPar)
{
    while (1)
    {
        char string[20] = "";
        sprintf(string, "%d", rand());
        if (xSemaphoreTake(mutex, (TickType_t) 10) == pdTRUE){  
            strcpy(text, string);
            printf("Write %s\n", string);
            xSemaphoreGive(mutex);
        }else{
            printf("Error: Could not get Semaphore\n");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }  
}

void app_main(void)
{
    printf("Start\n");
    mutex = xSemaphoreCreateMutex();
    srand(time(NULL));
    for (int i = 0; i < 3; i++)
    {
        xTaskCreate(&writer_task, "Writer Task", 2048, (void*) i, 1, NULL);
        xTaskCreate(&reader_task, "Reader Task", 2048, NULL, 1, NULL);
    }
    
    while (1){
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}