#include "mqtt_client.h"

static void mqtt_event_handler(void *arg, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            esp_mqtt_client_subscribe(client, "/topic/test", 0);
            esp_mqtt_client_publish(client, "/topic/test", "hello", 0, 0, 0);
            break;
        case MQTT_EVENT_DATA:
            printf("Topic: %.*s\n", event->topic_len, event->topic);
            printf("Data:  %.*s\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_DISCONNECTED:
            // handle reconnect logic if needed
            break;
    }
}

void mqtt_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://192.168.1.100",  // your broker
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}