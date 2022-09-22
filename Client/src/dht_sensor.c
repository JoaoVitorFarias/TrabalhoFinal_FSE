#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJSON.h"
#include "mqtt.h"
#include "dht11.h"
#include "dht_sensor.h"

void handle_dht(void * params){

    while(true)
    {
      struct dht11_reading dht11_data;
      dht11_data = DHT11_read();

      if (dht11_data.status == DHT11_OK){

        cJSON* response= cJSON_CreateObject();
        if (response == NULL){
              ESP_LOGE("DHT11", "Erro ao criar o json");
        }
      
        cJSON_AddItemToObject(response, "temperatura", cJSON_CreateNumber(dht11_data.temperature));
        cJSON_AddItemToObject(response, "umidade", cJSON_CreateNumber(dht11_data.humidity));

        mqtt_envia_mensagem("v1/devices/me/telemetry", cJSON_Print(response));
      }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void init_dht(){
    DHT11_init(GPIO_NUM_18);
    xTaskCreate(&handle_dht, "Leitura DHT11", 4096, NULL, 1, NULL);
}