#include "esp_partition.h"
#include "esp_spi_flash.h"
#include "utils.h"

namespace sys
{
    Hardware hw;
    Config config;

    void lspart()
    {
        const esp_partition_t *part = nullptr;
        esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);
        while (it != NULL)
        {
            part = esp_partition_get(it);
            Serial.printf(" - %-12s Offset: 0x%06X Size: %.2f MB\n",
                          part->label, part->address, part->size / 1048576.0);
            it = esp_partition_next(it);
        }

        it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
        while (it != NULL)
        {
            part = esp_partition_get(it);
            // Serial.printf(" - %-12s Offset: 0x%06X Type: 0x%02X Sub: 0x%02X Size: %.2f MB\n",
            //               part->label, part->address, part->type, part->subtype, part->size / 1048576.0);
            Serial.printf(" - %-12s Offset: 0x%06X Size: %.2f MB\n",
                          part->label, part->address, part->size / 1048576.0);
            it = esp_partition_next(it);
        }
    }

    void info()
    {
        float heap_size = ESP.getHeapSize() / 1048576.0;
        float heap_free = ESP.getFreeHeap() / 1048576.0;
        float flash_size = ESP.getFlashChipSize() / 1048576.0;
        float sketch_size = ESP.getSketchSize() / 1048576.0;
        float sketch_free = ESP.getFreeSketchSpace() / 1048576.0;
        float psram_size = ESP.getPsramSize() / 1048576.0;
        float psram_free = ESP.getFreePsram() / 1048576.0;

        Serial.println(F("[Sys] System info:"));

        Serial.printf(" - Flash:  %.2f MB\n", flash_size);

        Serial.printf(" - Heap:   %.2f/%.2f MB %.1f%%\n",
                      heap_size - heap_free, heap_size, (heap_size - heap_free) * 100.0 / heap_size);

        Serial.printf(" - Sketch: %.2f/%.2f MB %.1f%%\n", sketch_size, sketch_free, sketch_size * 100.0 / sketch_free);

#if defined(BOARD_HAS_PSRAM)
        float psram_used = (psram_size - psram_free) / 1024.0 / 1024.0;
        float psram_total = psram_size / 1024.0 / 1024.0;
        Serial.printf(" - PSRAM:  %.2f/%.2f MB\n", psram_used, psram_total);
#endif
    }

}