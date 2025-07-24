// #include "utils.h"
// #include <SPIFFS.h>

// static const char *CONF_FILE = "/conf.json";
// static constexpr size_t CONF_BUFF = 4096;
// namespace conf
// {
//     bool load(JsonDocument &doc)
//     {
//         if (!SPIFFS.begin(true))
//         {
//             Serial.println("[CONF] SPIFFS mount failed");
//             return false;
//         }

//         File file = SPIFFS.open(CONF_FILE, "r");
//         if (!file)
//         {
//             Serial.println("[CONF] File open failed");
//             return false;
//         }

//         DeserializationError err = deserializeJson(doc, file);
//         file.close();
//         if (err)
//         {
//             Serial.printf("[CONF] JSON parse failed: %s\n", err.c_str());
//             return false;
//         }
//         return true;
//     }

//     bool set(const char* path, JsonVariantConst value)
//     {
//         DynamicJsonDocument doc(CONF_BUFF);
//         if (!load(doc))
//             return false;
//         doc[path].set(value);
//         return save(doc);
//     }

//     bool save(const JsonDocument &doc)
//     {
//         File file = SPIFFS.open(CONF_FILE, "w");
//         if (!file)
//             return false;
//         serializeJsonPretty(doc, file);
//         file.close();
//         return true;
//     }

//     JsonVariant get(const char *path)
//     {
//         static DynamicJsonDocument doc(CONF_BUFF);
//         if (!load(doc))
//             return JsonVariant();
//         return doc[path];
//     }

//     bool ensure()
//     {
//         if (!SPIFFS.begin(true))
//             return false;

//         if (!SPIFFS.exists(CONF_FILE))
//         {
//             Serial.println("[CONF] No config found. Writing default...");

//             DynamicJsonDocument doc(CONF_BUFF);
//             doc["net"]["sta"]["connection"][0]["ssid"] = "defaultSSID";
//             doc["net"]["sta"]["connection"][0]["pass"] = "defaultPASS";

//             return save(doc);
//         }
//         return true;
//     }
// }