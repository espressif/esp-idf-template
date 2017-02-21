// ESP components
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "nvs_flash.h"

// Limbo components
#include "heartbeat.h"
#include "limbo_wifi.h"

void app_main(void)
{
  nvs_flash_init();
  xTaskCreate(&heartbeat_task, "heartbeat_task", 512, NULL, 5, NULL);

  limbo_wifi_init();
  
}

