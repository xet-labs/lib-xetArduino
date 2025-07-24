#include <new>
#include <string.h>
#include "pins.h"

// Internal storage
int (*MOTORS)[3]    = nullptr;
int (*MOTORSG)[6]   = nullptr;
int *SERVOS         = nullptr;
int (*STEPPERS)[2]  = nullptr;

// Counts
int NUM_MOTORS      = 0;
int NUM_MOTORSG     = 0;
int NUM_SERVOS      = 0;
int NUM_STEPPERS    = 0;

// Access
L298N* motor            = nullptr;
L298NX2* motorG         = nullptr;
Servo* servo            = nullptr;
AccelStepper* stepper   = nullptr;

// Register functions
#define MEM_DIFFERENT_OR_NULL(ptr, data, size) (!(ptr) || memcmp(ptr, data, size) != 0)

bool register_motors(int count, const int pins[][3]) {
    if (count <= 0 || !pins) return false;

    size_t size = count * sizeof(int[3]);

    if (MEM_DIFFERENT_OR_NULL(MOTORS, pins, size)) {
        free(MOTORS);
        MOTORS = static_cast<int(*)[3]>(malloc(size));
        if (!MOTORS) return false;
        memcpy(MOTORS, pins, size);
        NUM_MOTORS = count;
    }

    return true;
}

bool register_motorsG(int count, const int pins[][6]) {
    if (count <= 0 || !pins) return false;

    size_t size = count * sizeof(int[6]);

    if (MEM_DIFFERENT_OR_NULL(MOTORSG, pins, size)) {
        free(MOTORSG);
        MOTORSG = static_cast<int(*)[6]>(malloc(size));
        if (!MOTORSG) return false;
        memcpy(MOTORSG, pins, size);
        NUM_MOTORSG = count;
    }

    return true;
}

bool register_servos(int count, const int pins[]) {
    if (count <= 0 || !pins) return false;

    size_t size = count * sizeof(int);

    if (MEM_DIFFERENT_OR_NULL(SERVOS, pins, size)) {
        free(SERVOS);
        SERVOS = static_cast<int*>(malloc(size));
        if (!SERVOS) return false;
        memcpy(SERVOS, pins, size);
        NUM_SERVOS = count;
    }

    return true;
}

bool register_steppers(int count, const int pins[][2]) {
    if (count <= 0 || !pins) return false;

    size_t size = count * sizeof(int[2]);

    if (MEM_DIFFERENT_OR_NULL(STEPPERS, pins, size)) {
        free(STEPPERS);
        STEPPERS = static_cast<int(*)[2]>(malloc(size));
        if (!STEPPERS) return false;
        memcpy(STEPPERS, pins, size);
        NUM_STEPPERS = count;
    }

    return true;
}

void init_motors(int count)
{
    if (MOTORS != nullptr){
        if (count < 0) count = NUM_MOTORS;

        if (count != 0){
            if (motor != nullptr) {
                free_motors();  // Reuse cleanup logic
            }

            motor = (L298N*)malloc(sizeof(L298N) * count);
            if (!motor){
                Serial.println(F("[MOTOR] ERR Memory allocation failed"));
                return;
            }

            for (int i = 0; i < count; i++){
                new (&motor[i]) L298N(MOTORS[i][0], MOTORS[i][1], MOTORS[i][2]);
            }
        }

        Serial.printf(" - init %d Motor%s\n", count, count == 1 ? "" : "s");
    } else {
        Serial.println(F(" - init 0 Motor"));
    }
}

void init_motorsG(int count) {
    if (MOTORSG != nullptr) {
        if (count < 0) count = NUM_MOTORSG;
        if (count == 0) {
            Serial.println(F(" - init 0 Motor Groups"));
            return;
        }

        if (motorG != nullptr) {
            free_motorsG();
        }

        motorG = (L298NX2*)malloc(sizeof(L298NX2) * count);
        if (!motorG) {
            Serial.println(F("[MOTORG] ERR Memory allocation failed"));
            return;
        }

        for (int i = 0; i < count; i++) {
            new (&motorG[i]) L298NX2(MOTORSG[i][0], MOTORSG[i][1], MOTORSG[i][2], MOTORSG[i][3], MOTORSG[i][4], MOTORSG[i][5]);
        }

        Serial.printf(" - init %d Motor Group%s\n", count, count == 1 ? "" : "s");
    } else {
        Serial.println(F(" - init 0 Motor Groups"));
    }
}

void init_servos(int count)
{
    if (SERVOS != nullptr) {
        if (count < 0) count = NUM_SERVOS;

        if (count != 0) {
            if (servo != nullptr) {
                delete[] servo;
                servo = nullptr;
            }

            servo = new Servo[count];
            if (!servo) {
                Serial.println(F("[SERVO] ERR Memory allocation failed"));
                return;
            }

            for (int i = 0; i < count; i++) {
                servo[i].attach(SERVOS[i]);
            }
        }
        Serial.printf(" - init %d Servo%s\n", count, count == 1 ? "" : "s");
    } else {
        Serial.println(F(" - init 0 Servo"));
    }
}

void init_steppers(int count)
{
    if (STEPPERS != nullptr) {
        if (count < 0) count = NUM_STEPPERS;

        if (count != 0) {
            if (stepper != nullptr) {
                free(stepper);
                stepper = nullptr;
            }

            stepper = (AccelStepper*)malloc(count * sizeof(AccelStepper));
            if (!stepper) {
                Serial.println(F("[STEPPER] ERR Memory allocation failed"));
                return;
            }

            for (int i = 0; i < count; i++) {
                new (&stepper[i]) AccelStepper(AccelStepper::DRIVER, STEPPERS[i][0], STEPPERS[i][1]);
            }
        }
        Serial.printf(" - init %d Stepper%s\n", count, count == 1 ? "" : "s");
    } else {
        Serial.println(F(" - init 0 Stepper"));
    }
}

// Unified Init
void init_all_motors()
{
    Serial.println(F("[MOTORS] Initializing:"));
    init_motors();
    init_motorsG();
    init_servos();
    init_steppers();
}

// Motor A (simple L298N 3-pin)
void free_motors() {
    if (motor) {
        for (int i = 0; i < NUM_MOTORS; i++) {
            motor[i].~L298N();  // Explicitly destroy object
        }
        free(motor);
        motor = nullptr;
        Serial.println(" - freed Motors");
    }
}

// Motor G (extended L298N 6-pin)
void free_motorsG() {
    if (motorG) {
        for (int i = 0; i < NUM_MOTORS; i++) {
            motorG[i].~L298NX2();
        }
        free(motorG);
        motorG = nullptr;
        Serial.println(" - freed Motor Groups");
    }
}


// Servo
void free_servos() {
    if (servo) {
        for (int i = 0; i < NUM_SERVOS; i++) {
            servo[i].detach();  // optional: release control
        }
        delete[] servo;
        servo = nullptr;
        Serial.println(" - freed Servos");
    }
}

// Stepper (AccelStepper)
void free_steppers() {
    if (stepper) {
        for (int i = 0; i < NUM_STEPPERS; i++) {
            stepper[i].~AccelStepper();  // manually call destructor
        }
        free(stepper);
        stepper = nullptr;
        Serial.println(" - freed Steppers");
    }
}

// Unified Cleanup
void free_all_motors() {
    free_motors();
    free_motorsG();
    free_servos();
    free_steppers();
}
