//
// Created by Bartek on 01-Nov-24.
//
#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <array>
#define LED_PIN 15
#define LED_COUNT 16

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
class LedRing
{

  // std::array<bool, 16> leds;
  bool leds[16];
  uint32_t color;
  uint16_t convertCharToUint16(char id) {
    return id - '0';
  }

public:
  int r,g,b;
  bool switchLed(int id) {
    const auto led = static_cast<uint16_t>(id);
    if (leds[led] == true) {
        leds[led] = false;
        strip.setPixelColor(led, strip.Color(0, 0, 0));
        strip.show();
        return false;
    }
    else {
        leds[led] = true;
        return true;
    }
  }

  void clearLed(uint16_t led)
  {
    leds[led] = false;
  }

  void colorWipe(int wait)
  {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      if (leds[i])
      {
        strip.setPixelColor(i, this->color);
      }
      delay(wait);
      strip.show();
    }
  }

  void hexToRgb(uint32_t hexColor, uint8_t &r, uint8_t &g, uint8_t &b) {
    r = (hexColor >> 16) & 0xFF;
    g = (hexColor >> 8) & 0xFF;
    b = hexColor & 0xFF;
}

  void setColor(const uint8_t &r, const uint8_t &g, const uint8_t &b){  
    this->color = strip.Color(r,g,b);
    this->r = r;
    this->g = g;
    this->b = b;
  }

  std::string getHexColor() {
    std::ostringstream oss;
    oss << "#" << std::uppercase << std::setfill('0')
        << std::setw(2) << std::hex << r
        << std::setw(2) << std::hex << g
        << std::setw(2) << std::hex << b;

    return oss.str();
}

  bool getLedStatus(int index){
    return leds[index];
  }

  void setup()
  {
    strip.begin();
    strip.show();
    strip.setBrightness(250);
  }
};
