//#define USE_OCTOWS2811

#include "Arduino.h"
#include <FastLED.h>
#include <util/crc16.h>
#include "FatBike.h"
#include "sensors.h"
#include "Fire.h"

uint32_t frameCount = 0;
uint32_t sensorUpdateCount = 0;
uint8_t pixelOnGround = 0;
float bikeSpeedMph = 0.0F;
boolean isMovingMode = false;

Fire *fire;
FatBike *fatBike;

const uint32_t maxFps = 61;
const uint32_t microsPerFrame = 1000000 / maxFps;


void setup() {
    delay(3000);
    Serial.begin(115200);
    Serial.println(F("Starting..."));

    initSensors();
    addSensorEntropy();
//    Serial.println("s, yaw, pitch, roll");

//    test();

//    initFlash();
//    addSensorEntropy();

//    bikeSpeedMph = getBikeSpeedMph();
    pixelOnGround = getPixelOnGround();

    fire = &Fire::getInstance();
    fatBike = &FatBike::getInstance();
    addSensorEntropy();
    addSensorEntropy();
    addSensorEntropy();
    addSensorEntropy();
    addSensorEntropy();
    addSensorEntropy();
}

void loop() {
    uint32_t startMicros = micros();
    if (frameCount % 600 == 0) {
        addSensorEntropy();
        Serial.print(F("FPS: "));
        Serial.print(FastLED.getFPS());
        Serial.println(F(""));

        Serial.print(F("sensor updates/600 frames: "));
        Serial.println(sensorUpdateCount);
        sensorUpdateCount = 0;
    }

//BEGIN ANIMATION-SPECIFIC CODE

    if (sensorsUpdate()) {
        pixelOnGround = getPixelOnGround();
        sensorUpdateCount++;
    }
    fire->renderDoubleFire(pixelOnGround, bikeSpeedMph, isMovingMode, frameCount);
//    fatBike->renderSinglePixel(pixelOnGround);


//END ANIMATION-SPECIFIC CODE

    FastLED.show();
    fatBike->clearAll();
    frameCount++;

    uint32_t endMicros = micros();
    uint32_t elapsedMicros = endMicros - startMicros;
    if (elapsedMicros < microsPerFrame) {
        delayMicroseconds(microsPerFrame - elapsedMicros);
    }
}
