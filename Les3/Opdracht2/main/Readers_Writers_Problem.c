#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

SemaphoreHandle_t readerMutex;
SemaphoreHandle_t writerMutex;
int readercount = 0;
int cnt = 2;


void writer(void *pvParameters){
    while (1)
    {  
        if(xSemaphoreTake(writerMutex,10 ) == pdTRUE ){
            cnt += 2;
            //int index = (int) pvParameters;
            //printf("Writer %d : modified cnt to %d\n",index,cnt);
            printf("Writer : modified cnt to %d\n",cnt);
            xSemaphoreGive(writerMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void reader(void *pvParameters){
    int i = 0;
   while (1)
    {
        xSemaphoreTake(readerMutex,0);

        readercount++;

        if(readercount==1){
            printf("readcount = 1\n");
            if(xSemaphoreTake(writerMutex,0 ) == pdTRUE){
                printf("xSemaphoreTake = pdTRUE\n");
                i = 1;
            }
        }

        xSemaphoreGive(readerMutex);

        //int index = (int) pvParameters;
        //printf("Reader %d: read cnt as %d\n",index,cnt);
        printf("Reader: read cnt as %d\n",cnt);
        vTaskDelay(pdMS_TO_TICKS(1000));

        xSemaphoreTake(readerMutex,0);

        readercount--;

        printf("readcount before\n");
        if(readercount==0){
            //if(i){
            printf("readcount = 0\n");
              i = 0;
           xSemaphoreGive(writerMutex);
            //

            // xSemaphoreTake(writerMutex,portMAX_DELAY );
            // cnt += 2;
            // printf("Writer : modified cnt to %d\n",cnt);
            // xSemaphoreGive(writerMutex);
            // vTaskDelay(pdMS_TO_TICKS(100));
        
        }
        printf("readcount after\n");

        xSemaphoreGive(readerMutex);

        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("%d Reader is leaving\n",readercount+1);

    }
    
}

void init()
{
    int numReadersAndWriters = 3;

    readerMutex = xSemaphoreCreateMutex();
    writerMutex = xSemaphoreCreateMutex();

    for (int i = 0; i < numReadersAndWriters; i++)
    {
        xTaskCreate(&reader, "reader task", 2048, (void*)i, 0, NULL);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    for (int i = 0; i < numReadersAndWriters; i++)
    {
        xTaskCreate(&writer, "writer task", 2048, (void*)i, 5, NULL);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}

void app_main(void)
{  
    init();
    // while (1){
    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }
}
