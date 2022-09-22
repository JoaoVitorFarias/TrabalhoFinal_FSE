#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "mqtt.h"
#include "cJSON.h"
#include "button.h"

#define BOTAO 0

xQueueHandle filaDeInterrupcao;

char topic[60];

static void IRAM_ATTR gpio_isr_handler(void *args)
{
  int pino = (int)args;
  xQueueSendFromISR(filaDeInterrupcao, &pino, NULL);
}

void trataInterrupcaoBotao(void *params)
{
  int state = 0;
  while (true)
  {
    
      int estado = gpio_get_level(BOTAO);
      if (estado == 1)
      {
        gpio_isr_handler_remove(BOTAO);
        while(gpio_get_level(BOTAO) == estado)
        {
          vTaskDelay(50 / portTICK_PERIOD_MS);
        }

        if(state == 0){

          cJSON* response= cJSON_CreateObject();
          if (response == NULL){
              ESP_LOGE("ALARME", "Erro ao criar o json");
          }

          cJSON_AddItemToObject(response, "alarme", cJSON_CreateNumber(1));

          mqtt_envia_mensagem("v1/devices/me/attributes", cJSON_Print(response));

          vTaskDelay(50 / portTICK_PERIOD_MS);
          gpio_isr_handler_add(BOTAO, gpio_isr_handler, (void *) BOTAO);

          state = 1;

        }else{
          cJSON* response= cJSON_CreateObject();
          if (response == NULL){
              ESP_LOGE("ALARME", "Erro ao criar o json");
          }

          cJSON_AddItemToObject(response, "alarme", cJSON_CreateNumber(0));

          mqtt_envia_mensagem("v1/devices/me/attributes", cJSON_Print(response));

          state = 0;

        } 
      }
    
  }
}

void active_alarm()
{
  // Configuração dos pinos dos LEDs
  gpio_pad_select_gpio(BOTAO);

  // Configura o pino do Botão como Entrada
  gpio_set_direction(BOTAO, GPIO_MODE_INPUT);

  // Configura o resistor de Pulldown para o botão (por padrão a entrada estará em Zero)
  gpio_pulldown_en(BOTAO);

  // Desabilita o resistor de Pull-up por segurança.
  gpio_pullup_dis(BOTAO);

  // Configura pino para interrupção
  gpio_set_intr_type(BOTAO, GPIO_INTR_POSEDGE);

  filaDeInterrupcao = xQueueCreate(10, sizeof(int));
  xTaskCreate(&trataInterrupcaoBotao, "Tratar botão", 2048, NULL, 1, NULL);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(BOTAO, gpio_isr_handler, (void *)BOTAO);
}

void disable_alarm()
{
  gpio_isr_handler_remove(0);
  gpio_intr_disable(0);
  ESP_LOGI("BOTAO", "Alarme desativado\n");
}