#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "freertos/semphr.h"
#include "esp32/rom/uart.h"

#include "wifi.h"
#include "mqtt.h"
#include "cJSON.h"
#include "dht_sensor.h"
#include "flame_detector.h"
#include "button.h"

#include "driver/rtc_io.h"

#define MODO_BATERIA CONFIG_MODO_BATERIA
#define BOTAO 0

xSemaphoreHandle conexaoWifiSemaphore;
xSemaphoreHandle conexaoMQTTSemaphore;

void conectadoWifi(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      // Processamento Internet
      mqtt_start();
    }
  }
}

void handle_low_power(){

  gpio_pad_select_gpio(BOTAO);
  gpio_set_direction(BOTAO, GPIO_MODE_INPUT);


  gpio_wakeup_enable(BOTAO, GPIO_INTR_LOW_LEVEL);
  esp_sleep_enable_gpio_wakeup();

  ESP_LOGI("SYS", "SLEEPING");
  uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);
  esp_light_sleep_start();
  esp_restart();


}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&conectadoWifi,  "Conexão ao MQTT", 4096, NULL, 1, NULL);

    if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY)){
      if(MODO_BATERIA == 0){
        init_dht();
        init_detector();
        active_alarm();

      }
      else if(MODO_BATERIA == 1){
        handle_low_power();       
      }
    }
}
