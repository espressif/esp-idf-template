#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#if ESP_IDF_VERSION_MAJOR < 4
#include "esp_event_loop.h"
#endif
#include "nvs_flash.h"
#include "driver/gpio.h"

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 1, 0)
//
// From IDF 4.1.0 ESP-NETIF approach is encouraged and all network
//  interfaces have to be explicitely created
//
#define TCPIP_INIT()    esp_netif_init()
#define CREATE_WIFI_STATION()    assert(esp_netif_create_default_wifi_sta())
#else
#define TCPIP_INIT()    tcpip_adapter_init()
#define CREATE_WIFI_STATION()
#endif

#if ESP_IDF_VERSION_MAJOR >= 4

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
}

static void ip_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
}

#else

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

#endif

void app_main(void)
{
    nvs_flash_init();
    TCPIP_INIT();

#if ESP_IDF_VERSION_MAJOR >= 4
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_handler, NULL));
#else
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
#endif
    CREATE_WIFI_STATION();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
        .sta = {
            .ssid = CONFIG_ESP_WIFI_SSID,
            .password = CONFIG_ESP_WIFI_PASSWORD,
            .bssid_set = false
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );

    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    int level = 0;
    while (true) {
        gpio_set_level(GPIO_NUM_4, level);
        level = !level;
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

