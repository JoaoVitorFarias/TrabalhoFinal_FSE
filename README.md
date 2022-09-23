# FSE: Trabalho Final

Este é o trabalho final da disciplina de Fundamentos de Sistemas Embarcados (2022/1). O trabalho final pode ser feito em dupla ou individualmente.

## Descrição

O objetivo deste trabalho é criar sensores e atuadores distribuídos baseados nos microcontroladores ESP32 interconectados via Wifi através do protocolo MQTT, podendo ser aplicada em diversos contextos de automação a exemplo das áreas de Automação Residencial, Predial ou Industrial.

Os microcontroladores ESP32 irão controlar a aquisição de dados de sensores, botões e chaves e acionar saídas como leds, dentre outras. Haverão dois modos de operação dos dispositivos: modo energia que representa o dispositivo ligado à tomada e modo bateria que representa o dispositivo que deverá operar em modo de baixo consumo.

Toda a comunicação será feita via rede Wifi com o protocolo MQTT e será provido um servidor central para cadastro e controle dos dispositivos através da plataforma Thingsboard.

[Enunciado do Trabalho](https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-3-2022-1)

## Ambiente

Para o desenvolvimento do projeto foi utilizado o VSCode em conjunto com o [Platformio](https://platformio.org/). Além disso, foi utilizado a ESP32, o sensor DHT11 e o detector de chamas.

### Configuração dos sensores:

- O sensor DHT11 está conectado na GPIO18 da ESP32.
- No detector de chamas, a saída analógica está conectada na GPIO33 e a saída digital está conectada na GPIO13.

### Configuração do projeto:

- Clone o repositório ``` git clone https://github.com/JoaoVitorFarias/TrabalhoFinal_FSE.git```.
- Abra o Platformio no VSCode clique em *open*, em seguida *Open Project*, navegue até a pasta do trabalho e clique em *Open*.
- Agora é preciso acessar o *menuconfig* do projeto e definir as credenciais de acesso do Wifi(SSID e senha) e definir se o modo bateria estará ativo. Para acessar o *menuconfig* basta rodar o seguinte comando ```pio run -t menuconfig``` ou na barra lateral do Platformio navegando em ```esp32doit-devkit-v1 -> Platform -> Run Menuconfig```. Com isso, o ambiente do *menuconfig* será apresentado no terminal e será possível realizar as definições necessárias.
- Para finalizar a configuração do projeto é preciso gerar a *build*, para isso é só clicar no 'símbolo de check' na parte inferior do VSCode.

## Execução

Com os sensores conectados na ESP32 e com o projeto configurado no Platformio, é necessário:
- Conectar a ESP32 no computador através da porta USB.
- E realizar o *upload* do código na ESP32, para isso basta clicar na 'seta' na parte inferior do VSCode.

Desta forma, o trabalho estará sendo executado na ESP32 e mandando as mensagens para o *dashboard*.

## Apresentação

Segue o link da apresentação do trabalho [Link](https://youtu.be/kUtUWZopYpE)