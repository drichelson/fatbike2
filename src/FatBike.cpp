#include "FatBike.h"

FatBike::FatBike() {
    LEDS.addLeds<APA102, BGR>(leds, NUM_LEDS);
    pinMode(7, OUTPUT);
    digitalWrite(7, HIGH);  // enable access to LEDs
    LEDS.setCorrection(TypicalLEDStrip);
    LEDS.setMaxRefreshRate(MAX_FPS, true);
    LEDS.setBrightness(GLOBAL_BRIGHTNESS);
    LEDS.countFPS(1000);
    testPattern();
}

float FatBike::Forward1(float thisPixel) {
    return Forward(thisPixel, 1.0);
}

// Forward = same direction as motion of wheel (see arrow on tire)
float FatBike::Forward(float thisPixel, float shiftAmount) {
    float newPosition = thisPixel + shiftAmount;
    if (newPosition < NUM_LEDS) {
        return newPosition;
    }
    else {
        return newPosition - NUM_LEDS;
    }
}

float FatBike::Back1(float thisPixel) {
    return Back(thisPixel, 1.0);
}

float FatBike::Back(float thisPixel, float shiftAmount) {
    float newPosition = thisPixel - shiftAmount;
    if (newPosition >= 0) {
        return newPosition;
    }
    else {
        return newPosition + NUM_LEDS;
    }
}

void FatBike::clearAll() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
}

void FatBike::testPattern() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::White;
        FastLED.show();
        delay(2);
        leds[i] = CRGB::Black;
    }
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(2);
        leds[i] = CRGB::Black;
    }
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Green;
        FastLED.show();
        delay(2);
        leds[i] = CRGB::Black;
    }
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(2);
        leds[i] = CRGB::Black;
    }
}

void FatBike::renderSinglePixel(uint8_t pixel) {
    leds[pixel] = CRGB::Green;
}

