#include "NXPMotionSense/NXPMotionSense.h"
//#include <MahonyAHRS.h>
NXPMotionSense imu;
NXPSensorFusion filter;

#define PIXELS_BETWEEN_SENSOR_AND_ZERO_PIXEL 20.0F


//Mahony filter;

float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;
float roll, pitch, yaw;



long loopCount = 0;
long start = 0;
long elapsed = 0;

void initSensors() {
    imu.begin();
    filter.begin(100);
}


void test() {
    if (imu.available()) {

        start = micros();
        // Read the motion sensors
        imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

        // Update the SensorFusion filter
        filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

        // print the heading, pitch and roll
        roll = filter.getRoll();
        pitch = filter.getPitch();
        yaw = filter.getYaw();

        elapsed += micros() - start;
//        if (loopCount % 100 == 0) {
//            Serial.print(F("sensor time averaged over past 100 readings (micros):  "));
//            Serial.println(elapsed / 100.0);
            elapsed = 0;
            Serial.print("Yaw: ");
            Serial.print(yaw);
            Serial.print(" Pitch:  ");
            Serial.print(pitch);
            Serial.print(" Roll:  ");
            Serial.println(roll);
//        }


//        loopCount++;
    }
}

float sensorPitchTo360Scale(float sensorDegrees) {
    if (sensorDegrees > 0) {
        return 360.0F - sensorDegrees;
    }
    else {
        return -sensorDegrees;
    }
}


uint8_t getPixelOnGround(float mph) {
//    long startMicros = micros();
    imu.readMotionSensor(ax, ay, az, gx, gy, gz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    pitch = filter.getPitch();
    Serial.print("pitch: ");
    Serial.println(pitch);

    roll = filter.getRoll();
    Serial.print("roll: ");
    Serial.println(roll);


    yaw = filter.getYaw();
    Serial.print("yaw: ");
    Serial.println(yaw);

    float normalizedDegrees = sensorPitchTo360Scale(pitch);

    uint8_t thisPixel = (uint8_t) ((normalizedDegrees / 360.0F) * (NUM_LEDS - 1));

//    float timeMicros = micros() - startMicros ;
//    Serial.print(F("Accel getPixelOnGround time (micros):  "));
//    Serial.println(timeMicros);

    int speedFactor = mph * 3.0; //magic!
    return (uint8_t) FatBike::Forward(thisPixel, PIXELS_BETWEEN_SENSOR_AND_ZERO_PIXEL - speedFactor);
//    return (uint8_t) digitalSmooth(pixelOnGround, pixelOnGroundSmoothingArray);
}

uint8_t getPixelOnGround() {
    return getPixelOnGround(0);
}