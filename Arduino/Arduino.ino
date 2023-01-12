#include "defines.h"
#include <DHT.h>
#include <WiFi.h>
#include <AsyncMqtt_Generic.h>
#include <avr/dtostrf.h>

using namespace rtos;
Thread connectThread;

// DHT11 sensor on digital pin 6
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define MQTT_HOST         IPAddress(192, 168, 0, 101)
#define MQTT_PORT         1883

const char *PubTopic  = "data"; // Topic to publish

AsyncMqttClient mqttClient;

bool connectedWiFi  = false;
bool connectedMQTT  = false;

bool connectToWifi()
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    // don't continue
    while (true);
  }

  #define MAX_NUM_WIFI_CONNECT_TRIES_PER_LOOP       20

  uint8_t numWiFiConnectTries = 0;

  // attempt to connect to WiFi network
  while ( (status != WL_CONNECTED) && (numWiFiConnectTries++ < MAX_NUM_WIFI_CONNECT_TRIES_PER_LOOP) )
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(500);
  }

  if (status != WL_CONNECTED)
  {
    // Restart for Portenta as something is very wrong
    NVIC_SystemReset();
  }
  connectedWiFi = (status == WL_CONNECTED);

  return (status == WL_CONNECTED);
}

// Check connection every 1s
#define MQTT_CHECK_INTERVAL_MS     1000

void connectToMqttLoop()
{
  while (true)
  {
    if ( (WiFi.status() == WL_CONNECTED) && (WiFi.RSSI() != 0) )
    {
      if (!connectedMQTT)
      {
        mqttClient.connect();
      }

      if (!connectedWiFi)
      {
        connectedWiFi = true;
      }
    }
    else
    {
      if (connectedWiFi)
      {
        connectedWiFi = false;
        connectToWifi();
      }
    }

    delay(MQTT_CHECK_INTERVAL_MS);
  }
}

void connectToMqtt()
{
  mqttClient.connect();
}


void setup()
{
  Serial.begin(115200);

  // Initialize the DHT11 sensor
  dht.begin();
  
  while (!Serial && millis() < 5000);

  connectToWifi();

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  // Add "connectToMqttLoop" loop to control connection To Mqtt
  connectThread.start(connectToMqttLoop);

  connectToMqtt();
}

void loop()
{
  // check kas siin töötab sensori andmete lugemine
  
  // Read the temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  char h_data[8]; // Buffer
  dtostrf(humidity, 6, 2, h_data); 

  char t_data[8]; // Buffer
  dtostrf(temperature, 6, 2, t_data); 

  char str[90];
  strcpy(str, "Humidity: ");
  strcat(str, h_data);
  strcat(str, " | Temperature: ");
  strcat(str, t_data);
 
  // publish
  mqttClient.publish(PubTopic, 0, false, str);
  // Wait a few seconds before making another measurement
  delay(2000);
}
