#include <Arduino.h>

const int PIN_TRIG = 26;
const int PIN_ECHO_1 = 34;
const int PIN_ECHO_2 = 14;
const int PIN_BTN_CALIB = 25; //calib

float baselineUS1 = 0.0; //calib baseline
float baselineUS2 = 0.0;

void setup() {
    Serial.begin(115200);

    pinMode(PIN_TRIG, OUTPUT); //US sensor

    digitalWrite(PIN_TRIG, LOW);

    pinMode(PIN_ECHO_1, INPUT);

    pinMode(PIN_ECHO_2, INPUT);

    pinMode(PIN_BTN_CALIB, INPUT_PULLUP); // calib

    Serial.println(F("Sensors activated and calibrated"));

}



float readUltrasonic(int echoPin) {

    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);

    if (duration == 0) {

        return -1.0; // Out of range

    }

    return (duration * 0.0343) / 2.0;

}  //US sensor calculation



void loop() {

    float rawUS1 = readUltrasonic(PIN_ECHO_1);
    float rawUS2 = readUltrasonic(PIN_ECHO_2);

    // apply calibration offsets if valid

    float distUS1 = (rawUS1 >= 0) ? (rawUS1 - baselineUS1) : -1.0;
    float distUS2 = (rawUS2 >= 0) ? (rawUS2 - baselineUS2) : -1.0;


    if (digitalRead(PIN_BTN_CALIB) == LOW) {

        if (rawUS1 >= 0 && rawUS2 >= 0) {
            baselineUS1 = rawUS1;
            baselineUS2 = rawUS2;
            Serial.println(F("--> [success] baseline updated"));
        } else {

            Serial.println(F("--> [error] out of range"));
            
        }
        delay(300);
    }


    Serial.printf("US1: %5.1f cm (Base: %.1f) | US2: %5.1f cm (Base: %.1f)\n",

                  distUS1, baselineUS1, distUS2, baselineUS2); //live readings on serial

    delay(200); 

}