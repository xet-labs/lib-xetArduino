#include "utils.h"

namespace controller
{
    namespace Ps3 
    {

        void printState(const controller::Ps3::State &stateData, const uint8_t *rawPacket, int rawPacketSize)
        {
            unsigned long ms = millis();
            int hours   = ms / 3600000;
            int minutes = (ms % 3600000) / 60000;
            int seconds = (ms % 60000) / 1000;
            int msec    = ms % 1000;

            char timestamp[20];
            snprintf(timestamp, sizeof(timestamp),
                    "%02d:%02d:%02d.%03d", hours, minutes, seconds, msec);

            char pressed[128] = "";
            bool first = true;

            const char *buttonNames[] = {
                "A","B","X","Y","LB","RB","Back","Start",
                "LStick","RStick","DPadUp","DPadDown","DPadLeft","DPadRight"
            };

            for (int bit = 0; bit < 14; bit++)
            {
                if (stateData.btns & (1 << bit))
                {
                    if (!first) strcat(pressed, ", ");
                    strcat(pressed, buttonNames[bit]);
                    first = false;
                }
            }

            if (first) strcpy(pressed, "None");

            Serial.printf(
                "%s  LX:%6d LY:%6d RX:%6d RY:%6d LT:%3d RT:%3d  Btns:0x%04X (%s)\n",
                timestamp,
                stateData.axisLX,
                stateData.axisLY,
                stateData.axisRX,
                stateData.axisRY,
                stateData.triggerLT,
                stateData.triggerRT,
                stateData.btns,
                pressed
            );

            if (rawPacket && rawPacketSize > 0)
            {
                util::printBytes(rawPacket, rawPacketSize); 
            }
        }
    }
}