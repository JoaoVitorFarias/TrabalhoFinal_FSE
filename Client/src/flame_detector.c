#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "cJSON.h"
#include "flash_nvs.h"
#include "mqtt.h"

#include"flame_detector.h"

#define DETECTOR_ADC ADC1_CHANNEL_5
#define DETECTOR_DAC ADC2_CHANNEL_4

#define GPIO_ADC 33
#define GPIO_DAC 13

int valueAdc;
int valueDac;

void init_detector()
{
    gpio_pad_select_gpio(GPIO_ADC);
    gpio_set_direction(GPIO_ADC, GPIO_MODE_INPUT);
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(DETECTOR_ADC, ADC_ATTEN_0db);

    gpio_pad_select_gpio(GPIO_DAC);
    gpio_set_direction(GPIO_DAC, GPIO_MODE_INPUT);

    int32_t detectorState = le_valor_nvs();
    mqtt_send_msg_flame(detectorState);

    xTaskCreate(&handle_detector, "Detector de chama", 4096, NULL, 1, NULL);
}

void handle_detector(void * params)
{
    valueAdc = 0;
    valueDac = 0;

    while (true)
    {
        valueAdc = adc1_get_raw(DETECTOR_ADC);
        valueDac = gpio_get_level(GPIO_DAC);

        ESP_LOGI("FOGO", "ADC: %d - DAC: %d",valueAdc, valueDac);

        if (valueDac != 0){
            ESP_LOGI("Detector de chamas", "Fogo detectado!");

            mqtt_send_msg_flame(valueDac);

            grava_valor_nvs(valueDac);

            vTaskDelay(2000 / portTICK_PERIOD_MS);

        }else {
            mqtt_send_msg_flame(valueDac);
            grava_valor_nvs(valueDac);

        }
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void mqtt_send_msg_flame(int state){

    cJSON* response= cJSON_CreateObject();
    if (response == NULL){
        ESP_LOGE("DETECTOR DE CHAMA", "Erro ao criar o json");
    }

    cJSON_AddItemToObject(response, "fogo", cJSON_CreateNumber(state));

    mqtt_envia_mensagem("v1/devices/me/attributes", cJSON_Print(response));

}

