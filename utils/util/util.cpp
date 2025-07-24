#include "../utils.h"
#include "../../lib.h"

// helper: print raw bytes
void printRawBytes(const uint8_t *data, size_t len)
{
    Serial.print("   Rx: ");
    for (size_t i = 0; i < len; ++i)
        Serial.printf("%02x", data[i]);
    Serial.println();
}