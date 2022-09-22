#ifndef MQTT_H
#define MQTT_H

void mqtt_start();
void handle_data(char *data);
void mqtt_envia_mensagem(char * topico, char * mensagem);

#endif