#include <Adafruit_NeoPixel.h>
#include "Thunder_Lightning.h"

#define LED_PIN 7
#define NUM_LEDS 1

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Thunder_Lightning lightning(&strip, 90, 255);

void setup()
{
    strip.begin();
    strip.show();
}

void loop()
{
    lightning.updateLightning();
}