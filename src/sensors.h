//#include <Mahony.h>
#include <NXPMotionSense.h>
//#include "MahonyAHRS.h"


//#include "NXPMotionSense/NXPMotionSense.h"

NXPMotionSense imu;

//slower:
NXPSensorFusion filter;

//faster:
//Mahony filter;

#define PIXELS_BETWEEN_SENSOR_AND_ZERO_PIXEL 119.0F
// observed extremes of ax accelerometer value
#define  AX_MAX 1.0f
#define AX_MIN (-1.0f)
//#define AX_MIN (-0.97f)


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

//TODO: optimize?
// returns 0-359
float getOrientation() {

    // roll
//    roll = atan2(y_Buff , z_Buff) * 57.3;

    // pitch
    float rawOrientation = atan2((-ax), sqrt(ay * ay + az * az)) * RAD_TO_DEG;
    float orientation;

    if (ax > 0.0) {
        // top hemishpere
        orientation = rawOrientation * 1.083032491F;
    } else {
        // bottom hemisphere
        orientation = rawOrientation * 1.043478261F;
    }

    if (ay >= 0.0F) {
        // right side
        return orientation + 90.0F;
    } else {
        // left side
        if (ax <= 0.0F) {
            // bottom left
            return 270.0F - orientation;
        } else {
            // top left
            orientation = 270.0F + (-1.0F * orientation);
            if (orientation >= 359.99F) {
                return 0.0F;
            }
            return orientation;
        }
    }
}


uint8_t getPixelOnGround(float mph) {
    float orientationDegrees = getOrientation();
//    Serial.print("Orientation: ");
//    Serial.print(orientationDegrees);
    float shiftedPixel = (orientationDegrees / 360.0F) * (NUM_LEDS - 1.0F);
//    Serial.print(", shiftedPixel: ");
//    Serial.println(shiftedPixel);
//    return static_cast<uint8_t>(shiftedPixel);
    // old code:
//    long startMicros = micros();
//    sensorsUpdate();

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

//    float normalizedDegrees = sensorPitchTo360Scale(pitch);

//    uint8_t thisPixel = (uint8_t) ((normalizedDegrees / 360.0F) * (NUM_LEDS - 1));

//    float timeMicros = micros() - startMicros ;
//    Serial.print(F("Accel getPixelOnGround time (micros):  "));
//    Serial.println(timeMicros);

//    int speedFactor = mph * 3.0; //magic!
    return (uint8_t) FatBike::Forward(shiftedPixel, NUM_LEDS / 2.0F);
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