#include <stdio.h>
#include "sdkconfig.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

//code internet 1 (works)

#include <unistd.h>
#include <pthread.h>

#define N_PHILOSOPHERS 5
#define LEFT (ph_num + N_PHILOSOPHERS - 1) % N_PHILOSOPHERS
#define RIGHT (ph_num + 1) % N_PHILOSOPHERS

pthread_mutex_t mutex;
pthread_cond_t condition[N_PHILOSOPHERS];

enum { THINKING, HUNGRY, EATING } state[N_PHILOSOPHERS];
int phil_num[N_PHILOSOPHERS];

void *philosophing (void *arg);
void pickup_forks(int ph_num);
void return_forks(int ph_num);
void test(int ph_num);
void nvs_flash_init();

void app_main()
{
nvs_flash_init();
  /* Setup */
  pthread_t ph_thread[N_PHILOSOPHERS];
  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < N_PHILOSOPHERS; i++) {
    pthread_cond_init(&condition[i], NULL);
    phil_num[i] = i;
  }

  /* Meat */
  for (int i = 0; i < N_PHILOSOPHERS; i++) {
    pthread_create(&ph_thread[i], NULL, philosophing, &phil_num[i]);
    printf("Philosopher #%d sits on the table.\n", i + 1);
    sleep(1);
  }
  for (int i = 0; i < N_PHILOSOPHERS; i++)
    pthread_join(ph_thread[i], NULL);

  /* Cleanup */
  pthread_mutex_destroy(&mutex);
  for (int i = 0; i < N_PHILOSOPHERS; i++)
    pthread_cond_destroy(&condition[i]);
}

void *philosophing(void *arg)
{
  while(1) {
    int *ph_num = arg;
    printf("Philosopher #%d starts thinking.\n", *ph_num + 1);
    sleep(2);
    pickup_forks(*ph_num);
    return_forks(*ph_num);
  }
}

void pickup_forks(int ph_num) {
  pthread_mutex_lock(&mutex);

  printf("Philosopher #%d is HUNGRY. She tries to grab her forks.\n", ph_num + 1);
  state[ph_num] = HUNGRY;
  test(ph_num);
  while (state[ph_num] != EATING) 
    pthread_cond_wait(&condition[ph_num], &mutex);

  pthread_mutex_unlock(&mutex);
}

void return_forks(int ph_num) {
  pthread_mutex_lock(&mutex);

  printf("Philosopher #%d puts down chopsticks. Now she asks her neighbors if they are hungry.\n", ph_num + 1);
  state[ph_num] = THINKING;
  test(LEFT);
  test(RIGHT);

  pthread_mutex_unlock(&mutex);
}

void test(int ph_num) {
  if (state[ph_num] == HUNGRY && 
      state[LEFT] != EATING && 
      state[RIGHT] != EATING) {
    printf("Philosopher #%d starts EATING.\n", ph_num + 1);
    state[ph_num] = EATING;
    sleep(3);
    pthread_cond_signal(&condition[ph_num]);
  }
}

//code internet 2 (works)

// #include <pthread.h> 
// //#include <semaphore.h> 
// //#include <stdio.h> 
  
// #define N 5 
// #define THINKING 2 
// #define HUNGRY 1 
// #define EATING 0 
// #define LEFT (phnum + 4) % N 
// #define RIGHT (phnum + 1) % N 

// void nvs_flash_init();
  
// int state[N]; 
// int phil[N] = { 0, 1, 2, 3, 4 }; 
  
// xSemaphoreHandle mutex; 
// xSemaphoreHandle S[N]; 
  
// void test(int phnum) 
// { 
//     if (state[phnum] == HUNGRY 
//         && state[LEFT] != EATING 
//         && state[RIGHT] != EATING) { 
//         // state that eating 
//         state[phnum] = EATING; 
  
//         sleep(2); 
  
//         printf("Philosopher %d takes fork %d and %d\n", 
//                       phnum + 1, LEFT + 1, phnum + 1); 
  
//         printf("Philosopher %d is Eating\n", phnum + 1); 
  
//         xSemaphoreGive(&S[phnum]); 
//     } else{
//         printf("Philosopher %d ERROR\n", phnum + 1); 
//     }
// } 
  
// // take up chopsticks 
// void take_fork(int phnum) 
// { 
//     xSemaphoreTake(&mutex, portMAX_DELAY); 
  
//     // state that hungry 
//     state[phnum] = HUNGRY; 
  
//     printf("Philosopher %d is Hungry\n", phnum + 1); 
  
//     // eat if neighbours are not eating 
//     test(phnum); 
  
//     xSemaphoreGive(&mutex); 
  
//     // if unable to eat wait to be signalled 
//     xSemaphoreTake(&S[phnum], portMAX_DELAY); 
  
//     sleep(1); 
// } 
  
// // put down chopsticks 
// void put_fork(int phnum) 
// { 
//     xSemaphoreTake(&mutex, portMAX_DELAY); 
  
//     // state that thinking 
//     state[phnum] = THINKING; 
  
//     printf("Philosopher %d putting fork %d and %d down\n", 
//            phnum + 1, LEFT + 1, phnum + 1); 
//     printf("Philosopher %d is thinking\n", phnum + 1); 
  
//     test(LEFT); 
//     test(RIGHT); 
  
//     xSemaphoreGive(&mutex); 
// } 
  
// void* philospher(void* num) 
// { 
  
//     while (1) { 
  
//         int* i = num; 
  
//         sleep(1); 
  
//       printf("Philosopher %n is testing\n", i + 1); 
//         take_fork(*i); 
  
//         sleep(0); 
  
//         put_fork(*i); 
//     } 
// } 
  
// void app_main()
// {
//     nvs_flash_init();

//     int i; 
//     pthread_t thread_id[N]; 
  
//     // initialize the semaphores 
//       mutex = xSemaphoreCreateMutex();

//     for (i = 0; i < N; i++) {
//       S[i] = xSemaphoreCreateMutex();
//     }
  
//     for (i = 0; i < N; i++) { 
  
//         // create philosopher processes 
//         pthread_create(&thread_id[i], NULL, 
//                        philospher, &phil[i]); 
  
//         printf("Philosopher %d is thinking\n", i + 1); 
//     } 
  
//     for (i = 0; i < N; i++) {
  
//         pthread_join(thread_id[i], NULL); 
//     }
// } 




//doesnt work ( my code )

// #define N 5 
// #define THINKING 2 
// #define HUNGRY 1 
// #define EATING 0 
// #define LEFT (ph_num + N - 1) % N
// #define RIGHT (ph_num + 1) % N

// int state[N]; 
// int phil[N] = { 0, 1, 2, 3, 4 }; 
  
// static SemaphoreHandle_t xSemaphore; 
// static SemaphoreHandle_t S[N];

// void nvs_flash_init();
 
// void check_neighbours(int num){

//     printf("check neighbours p = %d",num);

//     int LEFT = 0;
//     int RIGHT = 0;

//     LEFT = (num + 4) % N;
//     RIGHT = (num + 1) % N;

//     if(state[num] == HUNGRY 
//         && state[LEFT] != EATING 
//         && state[RIGHT] != EATING) { 
//         // state that eating 
//         state[num] = EATING; 

//         xSemaphoreGive(&S[num]);

//     }else{
//         printf("ERROR check neighbours p = %d",num);
//     }

// }

// void take_fork(int num){

//     xSemaphoreTake(&xSemaphore, 10);
   
//     state[num] = HUNGRY;

//     printf("take fork p = %d",num);

//     //ESP_LOGI("HUNGRY", "take fork");

//     check_neighbours(num);

//     xSemaphoreGive(&xSemaphore);
    
// }

// void put_fork(int num){

//     xSemaphoreTake(&xSemaphore, 10);

//     state[num] = THINKING;

//     printf("put fork p = %d",num);

//     //ESP_LOGI("THINK", "put fork");

//     int LEFT = 0;
//     int RIGHT = 0;

//     LEFT = (num + 4) % N;
//     RIGHT = (num + 1) % N;
//     check_neighbours(LEFT);
//     check_neighbours(RIGHT);

//     xSemaphoreGive(&xSemaphore);
// }

// void philosopher(void *pvParameters) 
// { 
  
//     while (1) { 
  
//         int* i = (int *) pvParameters; 
  
//         take_fork(*i); 
  
//         put_fork(*i); 
//     } 
// } 

// void app_main()
// {
//     nvs_flash_init();

//     xSemaphore = xSemaphoreCreateMutex();

//     if( xSemaphore != NULL ){

//         for (int i = 0; i < N; i++)
//         {
//            S[i] = xSemaphoreCreateMutex(); 
//            xTaskCreate(&philosopher, "philosopher", 1024, (void*) i, 1,NULL);          
//            //TickType_t xLastExecutionTime = xTaskGetTickCount();
//            //vTaskDelayUntil( &xLastExecutionTime, pdMS_TO_TICKS( 10 ) );
//         }

//     }

//     // vTaskStartScheduler();
    
//     for(;;);
// }









