//#define USE_OCTOWS2811

#include "Arduino.h"
//#include <OctoWS2811.h>
#include <FastLED.h>
#include <util/crc16.h>
#include "FatBike.h"
//#include "FatBike.cpp"
#include "sensors.h"
//#include "flash.h"
#include "Fire.h"

long frameCount = 0L;
uint8_t pixelOnGround = 0;
float bikeSpeedMph = 0.0F;
boolean isMovingMode = false;

Fire *fire;
FatBike *fatBike;


void setup() {
    // this breaks after platformio update:
//    _crc16_update(1, 0);
    delay(3000);
    Serial.begin(115200);
    Serial.println(F("Starting..."));

    initSensors();
//    Serial.println("s, yaw, pitch, roll");

//    test();

//    initFlash();
//    addSensorEntropy();

//    bikeSpeedMph = getBikeSpeedMph();
    pixelOnGround = getPixelOnGround();

    fire = &Fire::getInstance();
    fatBike = &FatBike::getInstance();

}

void loop() {
//    if (sensorsUpdate()){
//        Serial.println(getOrientation());
//    }
//    sensorsForVisualizer();
//    return;
//    sensorsUpdate();
//    if (frameCount % 100 == 0) {
//        Serial.println(pixelOnGround);
//        printSensorsForCsv();
//        printSensorsForArduinoPlotter();


//    return;

    if (frameCount % 100 == 0) {
        addSensorEntropy();
    }
//        Serial.print(F("FPS: "));
//        Serial.print(FastLED.getFPS());
//        Serial.println(F(""));

//    pitch = filter.getPitch();
//    Serial.print("Pitch: ");
//    Serial.print(pitch);

//    roll = filter.getRoll();
//    Serial.print(" Roll: ");
//    Serial.print(roll);

//    yaw = filter.getYaw();
//    Serial.print(" Yaw: ");
//    Serial.println(yaw);
//}

//BEGIN ANIMATION-SPECIFIC CODE

//    test();
    if (sensorsUpdate()) {
        pixelOnGround = getPixelOnGround();
//            fatBike->renderSinglePixel(pixelOnGround);
    }
    fire->renderDoubleFire(pixelOnGround, bikeSpeedMph, isMovingMode, frameCount);

//END ANIMATION-SPECIFIC CODE

    FastLED.show();
    fatBike->clearAll();
//    delay(50);

    frameCount++;

}
