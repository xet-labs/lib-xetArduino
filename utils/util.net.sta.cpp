#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include "utils.h"

namespace net::sta
{
    size_t maxCred = 12;
    size_t maxSsidLen = 32;
    size_t maxPassLen = 64;
    size_t credCount = 0;
    std::vector<std::pair<std::string, std::string>> creds;

    struct STA
    {
        String ssid;
        String bssid;
        int rssi;
        bool isCurrent;
        String ip;
    };

    void init(char *const args[])
    {
        const char *ssid = nullptr;
        const char *pass = nullptr;

        // CASE 1: No args → use default registered network
        if (!args || !args[0])
        {
            if (credCount == 0)
            {
                Serial.println("[STA] ERR: No WiFi credentials registered.");
                return;
            }

            ssid = creds[0].first.c_str();
            pass = creds[0].second.c_str();

            Serial.printf(F("[STA] Connecting to default: "));
        }
        // CASE 2: One arg → lookup SSID
        else if (args[0] && !args[1])
        {
            for (int i = 0; i < credCount; ++i)
            {
                if (strcmp(creds[i].first.c_str(), args[0]) == 0)
                {
                    ssid = creds[i].first.c_str();
                    pass = creds[i].second.c_str();
                    Serial.print(F("[STA] Connecting to registered: "));
                    break;
                }
            }

            if (!ssid)
            {
                Serial.print(F("[STA] ERR: SSID not found: "));
                Serial.println(args[0]);
                return;
            }
        }
        // CASE 3: Two or more args → use directly (ignore extras)
        else if (args[0] && args[1])
        {
            ssid = args[0];
            pass = args[1];
            Serial.print(F("[STA] Connecting to custom: "));
        }

            Serial.print(F("'"));
            Serial.print(ssid);     // single data for above print statement
            Serial.println(F("' "));

        // WiFi.setAutoReconnect(true);
        WiFi.begin(ssid, pass);

        if (WiFi.waitForConnectResult() == WL_CONNECTED)
        {
            Serial.print(F("\r[STA] Connected to '"));
            Serial.print(ssid);
            Serial.println(F("'"));
            info();
        }
        else
        {
            Serial.print(F("\r[STA] ERR: Couldn't connect to '"));
            Serial.print(ssid);
            Serial.println(F("'"));

            WiFi.disconnect(true);
        }

    }

    bool addConnection(const char *ssid, const char *pass)
    {
        if (!ssid || *ssid == '\0')
        {
            Serial.println("[STA] ERR: SSID is required.");
            return false;
        }

        size_t ssidLen = strlen(ssid);
        size_t passLen = pass ? strlen(pass) : 0;

        if (ssidLen > maxSsidLen)
        {
            Serial.printf("[STA] ERR: SSID too long (%zu > %zu): %s\n", ssidLen, maxSsidLen, ssid);
            return false;
        }
        if (passLen > maxPassLen)
        {
            Serial.printf("[STA] ERR: Password too long (%zu > %zu) for SSID: %s\n", passLen, maxPassLen, ssid);
            return false;
        }

        if (credCount >= maxCred)
        {
            Serial.println("[STA] ERR: Credential storage full.");
            return false;
        }

        creds.emplace_back(std::make_pair(std::string(ssid), pass ? std::string(pass) : ""));
        ++credCount;

        Serial.printf("[STA] Registered: %s (%s)\n", ssid, (pass && *pass) ? "with pass" : "open");
        return true;
    }

    bool addConnections(int arc, const char *arr[][2])
    {
        bool ok = true;
        for (int i = 0; i < arc; ++i)
        {
            if (!addConnection(arr[i][0], arr[i][1]))
            {
                ok = false;
            }
        }
        return ok;
    }

    void info()
    {
        String ssidStr = WiFi.SSID();
        String bssidStr = WiFi.BSSIDstr();
        int rssi = WiFi.RSSI();

        Serial.printf("[STA] SSID: %s | BSSID: %s | RSSI: %d | IP(l): %s",
                      ssidStr.c_str(),
                      bssidStr.c_str(),
                      rssi,
                      WiFi.localIP().toString().c_str());

        Serial.println();
    }

    void scan()
    {
        Serial.print(F("[STA] Scanning..."));
        int count = WiFi.scanNetworks();

        if (count <= 0)
        {
            Serial.print("\r                      \r");
            Serial.println("[STA] No WiFi available");
            return;
        }

        STA *list = new STA[count];

        size_t maxSSIDLen = 4;
        for (int i = 0; i < count; ++i)
        {
            list[i].ssid = WiFi.SSID(i);
            list[i].bssid = WiFi.BSSIDstr(i);
            list[i].rssi = WiFi.RSSI(i);
            list[i].isCurrent = (WiFi.SSID(i) == WiFi.SSID());
            list[i].ip = list[i].isCurrent ? WiFi.localIP().toString() : "";
            maxSSIDLen = max(maxSSIDLen, list[i].ssid.length());
        }

        // Header
        Serial.printf("\r[STA] Found '%d' WiFi network%s:\n", count, count == 1 ? "" : "s");
        Serial.printf("   %-*s  %-17s  %-5s  %s\n", maxSSIDLen, "SSID", "BSSID", "RSSI", "IP(l)");

        // List entries
        for (int i = 0; i < count; ++i)
        {
            Serial.printf(" %c %-*s  %-17s  %-5d  %s\n",
                          list[i].isCurrent ? '*' : '-',
                          maxSSIDLen,
                          list[i].ssid.c_str(),
                          list[i].bssid.c_str(),
                          list[i].rssi,
                          list[i].ip.c_str());
        }

        delete[] list;
    }

    void show()
    {
        Serial.println("[STA] Saved SSID credentials:");
        for (int i = 0; i < credCount; ++i)
        {
            Serial.printf("  [%d] %s / %s\n", i, creds[i].first.c_str(), creds[i].second.c_str());
        }
    }
}