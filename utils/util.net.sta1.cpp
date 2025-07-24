// #include <ArduinoJson.h>
// #include <FS.h>
// #include <SPIFFS.h>
// #include <WiFi.h>
// #include "utils.h"

// namespace net
// {
//     namespace sta
//     {
//         const int maxCred = 12;
//         const int maxSsidLen = 32;
//         const int maxPassLen = 64;
//         char creds[maxCred][2][maxPassLen]; // [ssid][pass]
//         int credCount = 0;

//         struct STA
//         {
//             String ssid;
//             String bssid;
//             int rssi;
//             bool isCurrent;
//             String ip;
//         };

//         struct StaCred
//         {
//             String ssid;
//             String pass;
//         };

//         bool registerCred(const char *ssid, const char *pass)
//         {
//             if (!SPIFFS.begin(true))
//             {
//                 Serial.println("[STA] ERR: SPIFFS mount failed");
//                 return false;
//             }

//             DynamicJsonDocument doc(4096);
//             File file = SPIFFS.open("/conf.json", "r");
//             if (file)
//             {
//                 deserializeJson(doc, file);
//                 file.close();
//             }

//             JsonArray conn = doc["net"]["sta"]["connection"];
//             if (!conn)
//             {
//                 conn = doc["net"]["sta"].createNestedArray("connection");
//             }

//             for (JsonObject c : conn)
//             {
//                 if (c["ssid"] == ssid)
//                 {
//                     c["pass"] = pass;
//                     goto save;
//                 }
//             }

//             JsonObject newC = conn.createNestedObject();
//             newC["ssid"] = ssid;
//             newC["pass"] = pass;

//         save:
//             file = SPIFFS.open("/conf.json", "w");
//             if (!file)
//                 return false;
//             serializeJsonPretty(doc, file);
//             file.close();

//             Serial.printf("[STA] Registered: %s\n", ssid);
//             return true;
//         }

//         bool unregisterCred(const char *ssid)
//         {
//             if (!SPIFFS.begin(true))
//                 return false;

//             DynamicJsonDocument doc(4096);
//             File file = SPIFFS.open("/conf.json", "r");
//             if (!file)
//                 return false;

//             deserializeJson(doc, file);
//             file.close();

//             JsonArray conn = doc["net"]["sta"]["connection"];
//             if (!conn)
//                 return false;

//             for (int i = 0; i < conn.size(); ++i)
//             {
//                 if (conn[i]["ssid"] == ssid)
//                 {
//                     conn.remove(i);
//                     file = SPIFFS.open("/conf.json", "w");
//                     serializeJsonPretty(doc, file);
//                     file.close();
//                     Serial.printf("[STA] Unregistered: %s\n", ssid);
//                     return true;
//                 }
//             }
//             return false;
//         }

//         bool loadCreds()
//         {
//             if (!SPIFFS.begin(true))
//                 return false;

//             File file = SPIFFS.open("/conf.json", "r");
//             if (!file)
//                 return false;

//             DynamicJsonDocument doc(4096);
//             DeserializationError err = deserializeJson(doc, file);
//             file.close();
//             if (err)
//                 return false;

//             JsonArray conn = doc["net"]["sta"]["connection"];
//             if (!conn)
//                 return false;

//             credCount = min((int)conn.size(), maxCred);
//             for (int i = 0; i < credCount; ++i)
//             {
//                 strlcpy(creds[i][0], conn[i]["ssid"] | "", maxSsidLen);
//                 strlcpy(creds[i][1], conn[i]["pass"] | "", maxPassLen);
//             }

//             Serial.printf("[STA] Loaded %d credentials\n", credCount);
//             return true;
//         }

//         bool saveCreds()
//         {
//             DynamicJsonDocument doc(4096);
//             JsonArray conn = doc["net"]["sta"].createNestedArray("connection");

//             for (int i = 0; i < credCount; ++i)
//             {
//                 JsonObject c = conn.createNestedObject();
//                 c["ssid"] = creds[i][0];
//                 c["pass"] = creds[i][1];
//             }

//             File file = SPIFFS.open("/conf.json", "w");
//             if (!file)
//                 return false;
//             serializeJsonPretty(doc, file);
//             file.close();
//             return true;
//         }



//         // void init_STA()
//         // {
//         //     Serial.print("[STA] Connecting...");
//         //     WiFi.setAutoReconnect(true);
//         //     WiFi.begin(ssid, pass);
//         //     if (WiFi.waitForConnectResult() != WL_CONNECTED)
//         //     {
//         //         Serial.print("\r                   \r");
//         //         Serial.print(F("[STA] ERR Couldnt connect : "));
//         //         Serial.println(ssid);
//         //     }
//         //     else
//         //     {
//         //         Serial.print("\r                   \r");
//         //         info();
//         //     }
//         // }



//     }
// }