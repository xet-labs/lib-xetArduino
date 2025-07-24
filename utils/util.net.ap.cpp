#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include "utils.h"

namespace net::ap
{
    Connection connection = {};

    void init()
    {
        WiFi.disconnect(true);

        Serial.print(F("[AP] Initializing..."));

        // Configure static IP
        if (WiFi.softAPConfig(connection.local_ip, connection.gateway_ip, connection.subnet))
        {
            if (!WiFi.softAP(connection.ssid, connection.pass, connection.channel, connection.hidden, connection.max_conn,
                             connection.ftm_responder, connection.auth_mode, connection.cipher))
            {
                Serial.println(F("\r[AP] Failed to start SoftAP"));
                return;
            }
            
            // Success feedback
            Serial.println(F("\r[AP] SoftAP started:"));
            Serial.printf(" - SSID : %s\n", connection.ssid);
            Serial.printf(" - PASS : %s\n", connection.pass);
            Serial.printf(" - IP   : %s\n", WiFi.softAPIP().toString().c_str());
            Serial.printf(" - CH   : %d\n", connection.channel);
        }
        else
        {
            Serial.println(F("\r[AP] Failed to configure IP settings"));
            return;
        }
    }

}