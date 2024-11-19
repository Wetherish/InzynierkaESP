//
// Created by Bartek on 01-Nov-24.
//
#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <utility>

class myMqtt
{
    const char *mqtt_broker = "192.168.0.45";
    const char *topic = "Led";
    const char *topic1 = "color";
    const char *topic2 = "Mobile";
    const char *topic3 = "LedStatus";
    const char *mqtt_username = "ledring";
    const char *mqtt_password = "lokomotywa";
    const char *ssid = "Koziolki";
    const char *password = "xxun7mZajccm";
    const int mqtt_port = 1883;
    WiFiClient espClient;
    PubSubClient client;

public:
    void setup(const std::function<void(char *, byte *, unsigned int)> &callback)
    {
        client = PubSubClient(espClient);
        Serial.begin(9600);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.println("Connecting to WiFi..");
        }
        Serial.println("Connected to the Wi-Fi network");
        client.setServer(mqtt_broker, mqtt_port);
        client.setCallback(callback);
        while (!client.connected())
        {
            String client_id = "LedRing";
            client_id += String(WiFi.macAddress());
            Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
            if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
            {
                Serial.println("broker connected");
            }
            else
            {
                Serial.print("failed with state ");
                Serial.print(client.state());
                delay(2000);
            }
        }
        client.subscribe(topic);
        client.subscribe(topic1);
        client.subscribe(topic2);
        client.subscribe(topic3);
    }

    void reconnect()
    {
        while (!client.connected())
        {
            Serial.print("Attempting MQTT connection...");
            if (client.connect("LedRing"))
            {
                Serial.println("connected");
            }
            else
            {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 5 seconds");
                delay(5000);
            }
        }
    }

    void sendMsg(const char* topic, const char* payload)
    {
        Serial.print("topic = ");
        Serial.print(topic);
        Serial.print("payload = ");
        Serial.print(payload);
        client.publish(topic, payload);
    }

    void loop(LedRing ledRing){
        client.loop();
        // reconnect();
        // ledRing.colorWipe(1110);
    }
};
