#include <Arduino.h>
#include "LedRing.hpp"
#include "mqtt.hpp"
#include <Adafruit_NeoPixel.h>
#include <cstring>// Required for 16 MHz Adafruit Trinket
#ifdef __AVR__
#include <avr/power.h> 
#endif
LedRing ledRing;

void processColor(byte* payload, unsigned int length, char message[]){
    for (int i = 0; i < length; i++) {
            message[i] = (char)payload[i];
        }
        int r, g, b;
        sscanf(message + 1, "%02x%02x%02x", &r, &g, &b);
        Serial.print(r);
        Serial.print(" ");
        Serial.print(g);
        Serial.print(" ");
        Serial.print(b); 
        ledRing.setColor(r,g,b);
}

char* combine(const char* topic, int id) {
    int bufferSize = std::strlen(topic) + 20; 
    char* combined = new char[bufferSize];
    
    std::sprintf(combined, "%s/%d", topic, id);
    
    return combined;
}

myMqtt mqtt;


static void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");

    char message[length + 1];
    for (int i = 0; i < length; i++) {
        message[i] = (char)payload[i];
    }
    message[length] = '\0';
    
    Serial.println(message);

    if (strcmp(topic, "Led") == 0) {
        int ledId = atoi(message);
        if (ledId < 16 and ledId )
        {
            auto status = ledRing.switchLed(ledId); 
            if (status){
                mqtt.sendMsg("LedResponse", combine("on",ledId));
            }else{
                mqtt.sendMsg("LedResponse", combine("off",ledId));
            }
        }
    } 
    else if (strcmp(topic, "color") == 0) {
        processColor(payload, length, message);
        mqtt.sendMsg("colorResponse", ledRing.getHexColor().c_str()); 
    }
    else if (strcmp(topic, "Mobile") == 0) {
        mqtt.sendMsg("Devices/0","online");
    }
    else if (strcmp(topic, "LedStatus") == 0) {
       int ledId = atoi(message);
        auto status = ledRing.getLedStatus(ledId); 
        if (status and ledId < 16 and ledId >= 0){
            mqtt.sendMsg("LedResponse", combine("on",ledId));
        }else{
            mqtt.sendMsg("LedResponse", combine("off",ledId));
        }        
        mqtt.sendMsg("colorResponse", ledRing.getHexColor().c_str()); 
    }
    

}

std::string response, topic;
void setup()
{
    ledRing.setup();
   
    mqtt.setup(callback);
    ledRing.setColor(0, 0, 50);

    // write your initialization code here
}

void loop()
{
    // colorWipe(strip.Color(0,   0,   255), 50); // Red
    // strip.setPixelColor(3, 255,   0,   0);         //  Set pixel's color (in RAM)
    mqtt.loop(ledRing);
    ledRing.colorWipe(12);
}