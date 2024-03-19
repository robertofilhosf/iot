# Projeto de IoT

### Alunos: 
- Ismael Marinho
- José Roberto
- Lavoisier Chaves

### Nome do projeto: Controle de garagem

## Objetivos
Desenvolver um sistema para uma casa inteligente automatizada e conectada visando proporcionar conforto e segurança aos moradores desta residência.
### Funcionalidades
Automatizar a abertura e o fechamento de um portão de garagem que, através de sensores, emitirá para uma plataforma móvel o estado em que se encontra o portão para que seja tomada a decisão pelo usuário de abrir, fechar ou deixar o portão no estado em que se encontra. Ainda sendo possível detectar quem o acionou.
- Automatizar o fechamento do portão
- Detectar presença de pessoas
- Controle de abertura
- Enviar informações do estado do portão para a plataforma

### Controla: Fluxo de entrada e saída da garagem.
### Monitoria: Estado do portão (aberto/fechado), com segurança.
## Arquitetura: Centralizada
Seria interligado um servidor central via wi-fi, onde se comunicaria com a aplicação, facilitando o monitoramento e o controle do dispostivo IoT.  
<img src="arquitetura.png" alt="arquiteture" width="600"/>
### Lista de Dispositivos IoT
- Sensor de abertura de porta
- Atuador de contato seco
- Sensor de movimento
- ESP32

### Protocolo: MQTT (Message Queuing Telemetry Transport)
Facilitar a troca de mensagens entre dispositivos IoT de forma confiável e assíncrona.
### Microcontrolador: ESP32
## Aplicação: ESP RainMaker

