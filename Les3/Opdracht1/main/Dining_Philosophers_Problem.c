#include <stdio.h>
#include "sdkconfig.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore;

void nvs_flash_init();
void philosopher(void *pvParameters);
 
void app_main()
{
    nvs_flash_init();

    xSemaphore = xSemaphoreCreateMutex();

    if( xSemaphore != NULL ){

    for (int i = 20; i > 0; i--)
    {
        xTaskCreate(&philosopher, "philosopher", 2048, NULL, i,NULL);
        TickType_t xLastExecutionTime = xTaskGetTickCount();
        vTaskDelayUntil( &xLastExecutionTime, pdMS_TO_TICKS( 10 ) );
    }

    }
    
}

void philosopher(void *pvParameters)
{
    xSemaphoreTake(xSemaphore, portMAX_DELAY);

    PRINTF("Hello, this is the ");

    taskYIELD();

    PRINTF("first task \r\n");

    xSemaphoreGive(xSemaphore);

    taskYIELD();
} 








//1 takes 1 and 2
//3 takes 3 and 4
//2,4,5 dont eat

//2 takes 2 and 3
//4 takes 4 and 5
//1,3,5 dont eat

//3 takes 3 and 4
//5 takes 5 and 1
//1,2,4 dont eat

//4 takes 4 and 5
//1 takes 1 and 2
//2,3,5 dont eat