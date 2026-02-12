// SPDX-License-Identifier: Apache-2.0
// Copyright 2025 Rishikesh Prasad
// This file is part of the SharedLib project.

#pragma once

#include <functional>
#include <string>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "util.hash.fnv1a.h"

namespace sys
{
    struct Hardware
    {
        int LED = 2;
        bool LED_LOCK = false;
        int BUZZER = 4;
        bool BUZZER_LOCK = false;
    };
    struct Config
    {
        std::string host_name = "xetep32";
    };

    extern Hardware hw;
    extern Config config;
    void info();
    void lspart();

    // namespace buzzer
    // {
    //     struct Config
    //     {
    //         uint8_t pin = hw.BUZZER;
    //         uint8_t channel = 0;
    //     };
        
    //     extern Config config;
    //     void begin(uint8_t pin, uint8_t channel = 0, uint16_t freq = 2000, uint8_t resolution = 10);
    //     void beep(int freq, int duration);

    //     void agree();
    //     void disagree();
    //     void success();
    //     void error();
    // }
}

namespace conf
{
    bool init();   // mount SPIFFS, ensure config exists
    bool load();   // load into internal doc
    bool save();   // save internal doc to SPIFFS
    bool reload(); // reload from SPIFFS again
    void clear();  // clear internal doc
    void print();  // debug print

    // JsonVariant get(const char *path);
    // bool set(const char *path, JsonVariantConst value);
    bool has(const char *path);
    bool remove(const char *path);

    // JsonDocument &data();
}

namespace handle
{
    extern String serialBuffer;
    void serial();
    void cmd(const char *cmd, char *const args[], size_t argc);
}

namespace lgc
{
    struct LogicCore
    {
        std::function<void()> fn;
        std::string label;

        void call() const
        {
            if (fn)
                fn();
        }
    };

    extern LogicCore core;
    const std::string &current();

    void list();
    void assign(const std::string &label);
    void add(std::function<void()> fn, const std::string &label);
}

namespace net
{
    namespace ap
    {
        struct Connection
        {
            const char *ssid = sys::config.host_name.c_str();
            const char *pass = "1219@EP41";
            int channel = 1;
            int hidden = 0;
            int max_conn = 6;
            bool ftm_responder = false;
            wifi_auth_mode_t auth_mode = WIFI_AUTH_WPA2_PSK;
            wifi_cipher_type_t cipher = WIFI_CIPHER_TYPE_CCMP;

            IPAddress local_ip = IPAddress(192, 168, 1, 1);
            IPAddress gateway_ip = IPAddress(192, 168, 1, 1);
            IPAddress subnet = IPAddress(255, 255, 255, 0);
        };

        extern Connection connection;
        void init();
    }

    namespace mdns
    {
        void init();
    }

    namespace sta
    {
        extern size_t maxCred;
        extern size_t maxSsidLen;
        extern size_t maxPassLen;
        extern size_t credCount;
        extern std::vector<std::pair<std::string, std::string>> creds;

        void init(char *const args[] = nullptr);
        void info();
        void scan();
        void showCreds();

        bool addConnection(const char *ssid, const char *pass);
        bool addConnections(int arc, const char *arr[][2]);
        bool loadCreds();
        bool saveCreds();
        bool unregisterCred(const char *ssid);
    }

    namespace tcp
    {
        struct Server
        {
            uint16_t port = 6000;
            WiFiServer srv;
            WiFiClient client;

            void begin(uint16_t _port);
            bool available();
            bool hasClient();
            void listen();
            String readLine();
        };

        extern Server server;
        void init(uint16_t port = 0);
    }

    namespace udp
    {
        struct Socket
        {
            uint16_t port = 8888;
            WiFiUDP udp;

            bool begin(uint16_t _port);
        };

        extern Socket socket;
        bool init(uint16_t port = 0);
    }

}



namespace util{
    void printRawBytes(const uint8_t *data, size_t len);
}
namespace btn
{
    namespace Joypad
    {
        struct __attribute__((packed)) State
        {
            uint16_t btns;
            int16_t axisLX;
            int16_t axisLY;
            int16_t axisRX;
            int16_t axisRY;
            uint8_t triggerLT;
            uint8_t triggerRT;
        };

        static_assert(sizeof(State) == 12, "Joypad::State must be 12 bytes");

        namespace Buttons
        {
            constexpr uint16_t A          = 1 << 0;
            constexpr uint16_t B          = 1 << 1;
            constexpr uint16_t X          = 1 << 2;
            constexpr uint16_t Y          = 1 << 3;
            constexpr uint16_t LB         = 1 << 4;
            constexpr uint16_t RB         = 1 << 5;
            constexpr uint16_t BACK       = 1 << 6;
            constexpr uint16_t START      = 1 << 7;
            constexpr uint16_t LS         = 1 << 8;
            constexpr uint16_t RS         = 1 << 9;
            constexpr uint16_t DPAD_UP    = 1 << 10;
            constexpr uint16_t DPAD_DOWN  = 1 << 11;
            constexpr uint16_t DPAD_LEFT  = 1 << 12;
            constexpr uint16_t DPAD_RIGHT = 1 << 13;
        }
    }
}
