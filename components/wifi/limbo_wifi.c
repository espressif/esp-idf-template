#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include "limbo_wifi.h"
#include "skynet.h"

static const char* TAG = "limbo_wifi";

esp_err_t event_handler(void *ctx, system_event_t *event)
{
  return ESP_OK;
}

void limbo_wifi_init(void)
{
  tcpip_adapter_init();
  ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
  ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
  wifi_config_t sta_config = {
    .sta = {
      .ssid = LIMBO_SSID,
      .password = LIMBO_PASSWORD,
      .bssid_set = false
    }
  };
  ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
  ESP_ERROR_CHECK( esp_wifi_start() );
  ESP_ERROR_CHECK( esp_wifi_connect() );

  ESP_LOGI(TAG, "Completed wifi init");
}
