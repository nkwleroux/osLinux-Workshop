#include <stdio.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t waiterMutex;
SemaphoreHandle_t forkMutex;
int availableForks = 5;
int pins[5] = {13,12,14,27,26};

int getNextPhilosopher()
{
    xSemaphoreTake(waiterMutex, 0);
    static int number = 0;
    if (number > 4)
    {
        number = 0;
    }
    int pin = pins[number];
    number++;
    xSemaphoreGive(waiterMutex);
    return pin;
}

int getForks(int amount)
{
    xSemaphoreTake(forkMutex, 0);
    if (amount <= availableForks){
        availableForks -= amount;
    }
    else{
        amount = 0;
    }
    xSemaphoreGive(forkMutex);
    return amount;
}

void returnForks(int amount)
{
    xSemaphoreTake(forkMutex, 0);
    availableForks += amount;
    xSemaphoreGive(forkMutex);
}

void waiter_task(void *pvParameters)
{
    int forks = 0;
    int selectedPhilosopher;
    while (1)
    {
        forks += getForks(2);
        if (forks >= 2)
        {
            selectedPhilosopher = getNextPhilosopher();
            printf("philosopher %d is eating with %d forks\n", selectedPhilosopher, 2);
            gpio_set_level(selectedPhilosopher, 1);
            vTaskDelay(pdMS_TO_TICKS(1000)); //nobody can eat spaghetti that fast.
            gpio_set_level(selectedPhilosopher, 0);
        }
        returnForks(forks);
        forks = 0;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void init()
{
    int pin[5] = {13,12,14,27,26};
    for (int i = 0; i < 5; i++)
    {
        gpio_pad_select_gpio(pin[i]);
        gpio_set_direction(pin[i], GPIO_MODE_OUTPUT);
    }
    waiterMutex = xSemaphoreCreateMutex();
    forkMutex = xSemaphoreCreateMutex();
    for (int i = 0; i < 5; i++)
    {
        xTaskCreate(&waiter_task, "philosopher task", 2048, NULL, 1, NULL);
    }
}

void app_main(void)
{  
    init();
    while (1){
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

