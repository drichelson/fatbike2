//#define USE_OCTOWS2811

#include "Arduino.h"
//#include <OctoWS2811.h>
#include <FastLED.h>
#include "FatBike.h"
//#include "FatBike.cpp"
#include "sensors.h"
#include "sensors_adafruit.h"
//#include "flash.h"
#include "Fire.h"

long frameCount = 0L;
uint8_t pixelOnGround = 0;
float bikeSpeedMph = 0.0F;
boolean isMovingMode = false;

Fire *fire;
FatBike *fatBike;


void setup() {
    delay(3000);
    Serial.begin(115200);
    Serial.println(F("Starting..."));

    initSensors();
    test();

//    initFlash();
//    addSensorEntropy();

//    bikeSpeedMph = getBikeSpeedMph();
//    pixelOnGround = getPixelOnGround(bikeSpeedMph);

    fire = &Fire::getInstance();
    fatBike = &FatBike::getInstance();
    initSensors();

}

void loop() {
    if (frameCount % 1000 == 0) {
//        addSensorEntropy();
        Serial.print(F("FPS: "));
        Serial.print(FastLED.getFPS());
        Serial.println(F(""));
    }

    //BEGIN ANIMATION-SPECIFIC CODE

//    test();
    //pixelOnGround = getPixelOnGround();
   fire->renderDoubleFire(pixelOnGround, bikeSpeedMph, isMovingMode, frameCount);

    //END ANIMATION-SPECIFIC CODE

    FastLED.show();
    fatBike->clearAll();
    frameCount++;
//    delay(500);
}
