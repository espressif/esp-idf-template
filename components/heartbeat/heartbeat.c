#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define HEARTBEAT_GPIO GPIO_NUM_5
#define HEARTBEAT_PERIOD_MS 250

void heartbeat_task(void *pvParameter)
{
  /* Configure the IOMUX register for pad HEARTBEAT_GPIO (some pads are
     muxed to GPIO on reset already, but some default to other
     functions and need to be switched to GPIO. Consult the
     Technical Reference for a list of pads and their default
     functions.)
  */
  gpio_pad_select_gpio(HEARTBEAT_GPIO);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(HEARTBEAT_GPIO, GPIO_MODE_OUTPUT);
  
  while(1) {
    /* Heartbeat off (output low) */
    gpio_set_level(HEARTBEAT_GPIO, 0);
    vTaskDelay(HEARTBEAT_PERIOD_MS / portTICK_PERIOD_MS);
    /* Heartbeat on (output high) */
    gpio_set_level(HEARTBEAT_GPIO, 1);
    vTaskDelay(HEARTBEAT_PERIOD_MS / portTICK_PERIOD_MS);
  }
}

