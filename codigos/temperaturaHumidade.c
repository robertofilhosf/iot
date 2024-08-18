#include <WiFi.h>
#include "PubSubClient.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define DHT_PIN 25
#define TEMP_PIN 26
#define DHT_TYPE DHT22   

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

// Temperature
OneWire tempOneWire(TEMP_PIN);  
DallasTemperature tempSensor(&tempOneWire); 
float temperatureValue;

// time and delay
uint16_t delayTime = 1000;
unsigned long currentTime = 0;
unsigned long lastTime = 0;

// DHT22
DHT dht(DHT_PIN, DHT_TYPE);
float humidity;

void setup() {
  Serial.begin(115200);

  wifiConnect();
  Serial.println("WiFi connected");

  client.setServer(mqttServer, PORT);
  client.setCallback(callback);
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

    tempSensor.requestTemperatures(); 
    temperatureValue = tempSensor.getTempCByIndex(0);
    
    humidity = dht.readHumidity(); 

    client.publish(TOPIC_PUBLISH_TEMPERATURE, String(temperatureValue).c_str());
    client.publish(TOPIC_PUBLISH_HUMIDITY, String(humidity).c_str());
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
  Serial.println(stMessage);
}
