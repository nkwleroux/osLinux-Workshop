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
static int i = 0;

//ONLY WORKS FIRST TIME. AFTER 4 DOENS'T WORK ANYMORE.

void writer(void *pvParameters){
    while (1)
    {  
        if(xSemaphoreTake(writerMutex, 100) == pdTRUE ){
            if(i){
            cnt += 2;
            int index = (int) pvParameters;
            printf("\nWriter %d : modified cnt to %d\n\n",index,cnt);
            //printf("Writer : modified cnt to %d\n",cnt);
            i = 0;
            xSemaphoreGive(writerMutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//dont

void reader(void *pvParameters){
 
    while (1)
    {
        if(xSemaphoreTake(readerMutex,100) == pdTRUE){

            readercount++;
    
            if(readercount==1){
                printf("readcount = 1\n");
                if(xSemaphoreTake(writerMutex, 0 ) == pdTRUE){
                    printf("xSemaphoreTake = pdTRUE\n");
                    i = 1;
                    readercount-=2;
                }
            }
    
            xSemaphoreGive(readerMutex);
        }

        int index = (int) pvParameters;
        printf("Reader %d: read cnt as %d\n",index,cnt);

        vTaskDelay(500);

        if(xSemaphoreTake(readerMutex,100) == pdTRUE){

            readercount--;

            printf("readcount %d before if\n",readercount);

            if(readercount==0){
                printf("readcount = 0\n");
                printf("i = %d\n",i);
                
                if(i){
                    printf("i = true - 1\n");
                    xSemaphoreGive(writerMutex);
                    
                }
            }

            xSemaphoreGive(readerMutex);
        }
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
        vTaskDelay(pdMS_TO_TICKS(1200));
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
