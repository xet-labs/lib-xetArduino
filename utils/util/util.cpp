#include "../utils.h"
#include "../../lib.h"

namespace util
{
    // helper: print raw bytes
    void printBytes(const uint8_t *data, size_t len)
    {
        if (len == 0) return;

        // Allocate a buffer large enough for 2 hex chars per byte plus null terminator
        char buf[len * 2 + 1]; 
        for (size_t i = 0; i < len; ++i) {
            sprintf(&buf[i * 2], "%02x", data[i]);
        }

        Serial.print("   Rx: ");
        Serial.println(buf);
        }
}