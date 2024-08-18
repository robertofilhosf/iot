#include <WiFi.h>
#include "PubSubClient.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Ultrasonic.h>

#define LIGHT_PIN 4
#define GATE_PIN 2
#define US_TRIGGER 19
#define US_ECHO 18

#define TOPIC_SUBSCRIBE_LIGHT       "IFPB/Iot/Projeto.Garagem/Luz"
#define TOPIC_PUBLISH_TEMPERATURE "IFPB/Iot/Projeto.Garagem/Temperatura"
#define TOPIC_PUBLISH_HUMIDITY    "IFPB/Iot/Projeto.Garagem/Umidade"
#define TOPIC_PUBLISH_SENSOR_GATE    "IFPB/Iot/Projeto.Garagem/Sensor.Portao"
#define TOPIC_PUBLISH_ACTUATOR_GATE    "IFPB/Iot/Projeto.Garagem/Atuador.Portao"

#define PORT 1883
#define ID_MQTT "Projeto-Garagem"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.emqx.io";

// WiFi/MQTT client 
WiFiClient espClient;
PubSubClient client(espClient);

// time and delay
uint16_t delayTime = 1000;
unsigned long currentTime = 0;
unsigned long lastTime = 0;

// Ultra Sonic
Ultrasonic u_s(US_TRIGGER, US_ECHO);
int distance;

void setup() {
  Serial.begin(115200);

  wifiConnect();
  Serial.println("WiFi connected");

  client.setServer(mqttServer, PORT);
  client.setCallback(callback);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(GATE_PIN, OUTPUT);
}

void loop() {
  currentTime = millis();
  
  delay(10);
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();
  
  if (currentTime - lastTime >= delayTime) {
    lastTime = currentTime;
    distance = u_s.read(CM);
    if (distance > 10) 
    {
      client.publish(TOPIC_PUBLISH_SENSOR_GATE, "open");
    } 
    else 
    {
      client.publish(TOPIC_PUBLISH_SENSOR_GATE, "close");
    }
  }
}

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT.");
    if (client.connect(ID_MQTT)) {
      Serial.println("Connected to MQTT.");
      client.subscribe(TOPIC_SUBSCRIBE_LIGHT);
      client.subscribe(TOPIC_PUBLISH_ACTUATOR_GATE);
    } 
    else {
      Serial.print("Failed to Connect to MQTT.");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  String stMessage;
  for (int i = 0; i < length; i++) {
    stMessage += (char)message[i];
  }
  if (String(topic) == TOPIC_SUBSCRIBE_LIGHT) 
  {
    if(stMessage == "on")
    {
      Serial.println("Turning the LED On");
      digitalWrite(LIGHT_PIN, HIGH);
    }
    else if(stMessage == "off")
    {
      Serial.println("Turning the LED Off");
      digitalWrite(LIGHT_PIN, LOW);
    }
  } 
  else if (String(topic) == TOPIC_PUBLISH_ACTUATOR_GATE) {
    if(stMessage == "toggle")
    {
      if (distance < 10) 
      {
        Serial.println("Opening the garage");
        digitalWrite(GATE_PIN, HIGH);
        delay(500);
        digitalWrite(GATE_PIN, LOW);
      } 
      else 
      {
        Serial.println("Closing the garage");
        digitalWrite(GATE_PIN, HIGH);
        delay(500);
        digitalWrite(GATE_PIN, LOW);
      }
    }
  }
}
