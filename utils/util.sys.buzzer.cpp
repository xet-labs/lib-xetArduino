// #include "utils.h"
// #include "../lib.h"
// #include <Arduino.h>

// namespace sys::buzzer
// {
//     Config config = {
//         .pin = static_cast<uint8_t>(hw.BUZZER),
//         .channel = 0
//     };

//     void begin(uint8_t pin, uint8_t channel, uint16_t freq, uint8_t resolution)
//     {
//         config.pin = pin;
//         config.channel = channel;

//         ledcSetup(channel, freq, resolution);
//         ledcAttachPin(pin, channel);
//     }

//     void beep(int freq, int duration)
//     {
//         ledcWriteTone(config.channel, freq);
//         delay(duration);
//         ledcWrite(config.channel, 0); // Stop
//         delay(50);
//     }

//     void agree()
//     {
//         beep(1500, 150);
//         delay(200);
//         beep(1500, 150);
//         delay(100);
//         beep(2000, 80);
//     }

//     void disagree()
//     {
//         beep(1000, 250);
//         delay(400);
//         beep(700, 100);
//         delay(100);
//         beep(700, 100);
//     }

//     void success()
//     {
//         beep(1000, 100);
//         beep(1300, 100);
//         beep(1600, 100);
//     }

//     void error()
//     {
//         beep(600, 500);
//     }
// }
