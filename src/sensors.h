#include <Mahony.h>
#include "NXPMotionSense/NXPMotionSense.h"

NXPMotionSense imu;

//slower:
//NXPSensorFusion filter;

//faster:
Mahony filter;

#define PIXELS_BETWEEN_SENSOR_AND_ZERO_PIXEL 20.0F



float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;
float roll, pitch, yaw;

uint32_t loopCount = 0;
uint32_t start = 0;
uint32_t elapsed = 0;

void sensorsUpdate();

void initSensors() {
    imu.begin();
    filter.begin(100);
    for (int i = 0; i < 1000; ++i) {
        sensorsUpdate();
    }
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


uint8_t getPixelOnGround(float mph) {
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

void sensorsUpdate() {
    imu.readMotionSensor(ax, ay, az, gx, gy, gz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
}

uint8_t getPixelOnGround() {
    return getPixelOnGround(0);
}
void sensorsForVisualizer(){
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
    float roll, pitch, heading;

    if (imu.available()) {
        // Read the motion sensors
        imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

        // Update the SensorFusion filter
        filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

        // print the heading, pitch and roll
        roll = filter.getRoll();
        pitch = filter.getPitch();
        heading = filter.getYaw();
        Serial.print("Orientation: ");
        Serial.print(heading);
        Serial.print(" ");
        Serial.print(pitch);
        Serial.print(" ");
        Serial.println(roll);
    }
}