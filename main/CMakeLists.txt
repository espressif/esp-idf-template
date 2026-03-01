idf_component_register(
    SRCS "main.c" "wifi.c" "mqtt.c"
    INCLUDE_DIRS "."
    REQUIRES mqtt esp_event esp_wifi nvs_flash esp_netif
)
