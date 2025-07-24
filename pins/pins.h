#pragma once

#include <AccelStepper.h>
#include <ESP32Servo.h>
#include <L298N.h>
#include <L298NX2.h>

// Pins
void conf_pins();
extern const int LED_onboard;

// --- Motor Config Arrays
extern int (*MOTORS)[3];
extern int (*MOTORSG)[6];
extern int* SERVOS;
extern int (*STEPPERS)[2];

// --- Motor Qty
extern int NUM_MOTORS;
extern int NUM_MOTORSG;
extern int NUM_SERVOS;
extern int NUM_STEPPERS;

// --- Motor Access
extern L298N* motor;
extern L298NX2* motorG;
extern Servo* servo;
extern AccelStepper* stepper;

// --- Motor Registrars
bool register_motors(int count, const int pins[][3]);
bool register_motorsG(int count, const int pins[][6]);
bool register_servos(int count, const int pins[]);
bool register_steppers(int count, const int pins[][2]);

// --- Motor Initializers
void init_all_motors();
void init_motors(int count = -1);
void init_motorsG(int count = -1);
void init_servos(int count = -1);
void init_steppers(int count = -1);

// --- Motor free
void free_all_motors();
void free_motors();
void free_motorsG();
void free_servos();
void free_steppers();