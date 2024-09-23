//#include <WiFi.h>
#include <ESP8266WiFi.h> 
#include "PubSubClient.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define DHT_PIN 4
#define TEMP_PIN 3
#define DHT_TYPE DHT22   

#define TOPIC_PUBLISH_TEMPERATURE "IFPB/Iot/Projeto.Garagem/Temperatura"
#define TOPIC_PUBLISH_HUMIDITY    "IFPB/Iot/Projeto.Garagem/Umidade"

#define PORT 1883
#define ID_MQTT "Projeto-Garagem"
const char* ssid = "ismaels";
const char* password = "876543210";
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

  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
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
