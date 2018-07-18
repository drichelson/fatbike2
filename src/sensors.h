//#include <Mahony.h>
#include <NXPMotionSense.h>
//#include "MahonyAHRS.h"


//#include "NXPMotionSense/NXPMotionSense.h"

NXPMotionSense imu;

//slower:
NXPSensorFusion filter;

//faster:
//Mahony filter;

#define PIXELS_BETWEEN_SENSOR_AND_ZERO_PIXEL 20.0F

float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;
// yaw == heading
float roll, pitch, yaw;

uint32_t loopCount = 0;
uint32_t start = 0;
uint32_t elapsed = 0;

boolean sensorsUpdate();
void initSensors() {
    Serial.println("initSensors()");
    Serial.println("imu.begin()");
    imu.begin();
    Serial.println("filter.begin()");
    filter.begin(100);
    Serial.print("Starting sensor warmup: ");
    Serial.println(millis());
//    for (int i = 0; i < 1000; ) {
//        if (sensorsUpdate()){
//            i++;
//        }
//    }
    Serial.print("Finished sensor warmup: ");
    Serial.println(millis());
}


void sensorTiming() {
    start = micros();

//    if (imu.available()) {
    // Read the motion sensors
    // takes ~1ms:
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);
    // Update the SensorFusion filter
    // takes 4.5-5.5ms using slower filter or nearly 0 with faster filter:
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
    // print the heading, pitch and roll
    elapsed += micros() - start;
    roll = filter.getRoll();
    pitch = filter.getPitch();
    yaw = filter.getYaw();
//    } else {
//        elapsed += micros() - start;
//
//        Serial.println("imu not available!!!");
//    }

    if (loopCount % 100 == 0) {
        Serial.print(F("sensor time averaged over past 100 readings (micros):  "));
        Serial.println(elapsed / 100.0);

        Serial.print("Yaw: ");
        Serial.print(yaw);
        Serial.print(" Pitch:  ");
        Serial.print(pitch);
        Serial.print(" Roll:  ");
        Serial.println(roll);
        elapsed = 0;
    }
    loopCount++;
}

float sensorPitchTo360Scale(float sensorDegrees) {
    if (sensorDegrees > 0) {
        return 360.0F - sensorDegrees;
    } else {
        return -sensorDegrees;
    }
}

// returns 0-359
uint16_t getOrientation() {
//    float ayMax = 1.0f;
//    float ayMin = -0.97f;
    // we're on a 'cardinal' direction
    if (ax > -0.01 && ax < 0.01) {
        if (ay > 0.0) return 90;
        else return 270;
    }
    if (ay > -0.01 && ay < 0.01) {
        if (ax > 0.0) return 0;
        else return 180;
    }

    if (ax > 0.0) {
        if (ay > 0.0) {
            return 45;
            // top right quadrant: 0-90 degrees
        } else {
            return 315;
            // top left quadrant: 270-360/0
        }
    } else {
        if (ay > 0.0) {
            return 135;
            // bottom right quadrant: 90-180 degrees
        } else {
            return 225;
            // bottom left quadrant: 180-270
        }
    }


}


uint8_t getPixelOnGround(float mph) {


    // old code:
//    long startMicros = micros();
    sensorsUpdate();

//    pitch = filter.getPitch();
//    Serial.print("Pitch: ");
//    Serial.println(pitch);

//    roll = filter.getRoll();
//    Serial.print(" Roll: ");
//    Serial.print(roll);
//
//    yaw = filter.getYaw();
//    Serial.print(" Yaw: ");
//    Serial.println(yaw);

    float normalizedDegrees = sensorPitchTo360Scale(pitch);

    uint8_t thisPixel = (uint8_t) ((normalizedDegrees / 360.0F) * (NUM_LEDS - 1));

//    float timeMicros = micros() - startMicros ;
//    Serial.print(F("Accel getPixelOnGround time (micros):  "));
//    Serial.println(timeMicros);

    int speedFactor = mph * 3.0; //magic!
    return (uint8_t) FatBike::Forward(thisPixel, PIXELS_BETWEEN_SENSOR_AND_ZERO_PIXEL - speedFactor);
//    return (uint8_t) digitalSmooth(pixelOnGround, pixelOnGroundSmoothingArray);
}

// Reads sensors and updates filter. Returns true if new data was read.
boolean sensorsUpdate() {
    if (imu.available()) {
        imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

        // Update the SensorFusion filter
//        filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
        return true;
    } else {
        return false;
    }

}

uint8_t getPixelOnGround() {
    return getPixelOnGround(0);
}

void sensorsForVisualizer() {
    if (sensorsUpdate()) {
        // print the heading, pitch and roll
//        yaw = filter.getYaw();
//        pitch = filter.getPitch();
//        roll = filter.getRoll();
        Serial.print(ax);
        Serial.print(",");
        Serial.print(ay);
        Serial.print(",");
        Serial.print(az);
        Serial.print(",");
        Serial.print(gx);
        Serial.print(",");
        Serial.print(gy);
        Serial.print(",");
        Serial.println(gz);
    }
}

void printSensorsForCsv() {
        // print the heading, pitch and roll
        yaw = filter.getYaw();
        pitch = filter.getPitch();
        roll = filter.getRoll();
        float elapsedSeconds = millis() / 1000.0f;
        Serial.print(elapsedSeconds);
        Serial.print(", ");
        Serial.print(yaw);
        Serial.print(",  ");
        Serial.print(pitch);
        Serial.print(",  ");
        Serial.println(roll);
}

void printSensorsForArduinoPlotter() {
        // print the heading, pitch and roll
//        yaw = filter.getYaw();
//        pitch = filter.getPitch();
        roll = filter.getRoll();
//        float elapsedSeconds = millis() / 1000.0f;
//        Serial.print(elapsedSeconds);
//        Serial.print(", ");
        Serial.print(yaw);
        Serial.print(" ");
//        Serial.print(pitch);
//        Serial.print(" ");
        Serial.println(roll);
}