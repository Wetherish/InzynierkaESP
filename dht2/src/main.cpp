#include <DHT.h>
#include <cstring> // Required for 16 MHz Adafruit Trinket
#include <bits/basic_string.h>

#define DHT_SENSOR_PIN D7 // The ESP8266 pin D7 connected to DHT11 sensor
#define MotionSensorPin D
#define DHT_SENSOR_TYPE DHT11


DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <bits/basic_string.h>
#include <Keypad.h>

const char *ssid = "Koziolki";
const char *password = "xxun7mZajccm";
const char *mqtt_server = "192.168.0.157";
const char *mqtt_username = "user1";
const char *mqtt_password = "lokomotywa";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


int8 key_layout[4][4] = {
  {1, 2, 3, 4},
  {5, 6, 7, 8},
  {9, 10,11,12},
  {13,14,15,16}
};

byte pin_rows[4] = {D0, D1, D2, D3};      // The ESP8266 pins connect to the row pins
byte pin_column[4] = {D4, D5, D6, D7};

void setup_wifi()
{

  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  randomSeed(micros());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  if (strcmp(topic, "Mobile") == 0)
  {
    client.publish("Devices/1", "online");
  }
}

void reconnect()
{
  while (!client.connected())
  {
    String clientId = "DHT-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
    {      
      client.subscribe("Mobile");
    }
    else
    {      
      delay(5000);
    }
  }
}

void setup()
{
  dht_sensor.begin();
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe("Mobile");
}

Keypad keypad = Keypad( makeKeymap(key_layout), pin_rows, pin_column, 4, 4 );


void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  float humi = dht_sensor.readHumidity();
  float temperature_C = dht_sensor.readTemperature();
  unsigned long now = millis();
  if (now - lastMsg > 2000)
  {
    lastMsg = now;
    // client.publish("outTopic", msg);
    if (!(isnan(temperature_C) || isnan(humi)))
    {    
      snprintf(msg, MSG_BUFFER_SIZE, "%f", humi);
      client.publish("Humidity", msg);
      snprintf(msg, MSG_BUFFER_SIZE, "%f", temperature_C);
      client.publish("Temperature", msg);
    }
  }
  auto key = keypad.getKey();
  if (key){
    if (key!=1)
        {
          snprintf(msg, MSG_BUFFER_SIZE, "%i", key-1);
          client.publish("Led",msg);  
        }
  }

}
