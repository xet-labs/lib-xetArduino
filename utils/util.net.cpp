#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include "utils.h"

namespace net
{
    namespace tcp
    {
        Server server;

        void init(uint16_t port)
        {
            if (port == 0)
                port = server.port;
            server.begin(port);
            Serial.print(F("[TCP] LPort: "));
            Serial.println(port);
        }

        void Server::begin(uint16_t _port)
        {
            this->port = _port;
            srv = WiFiServer(_port);
            srv.begin();
        }

        bool Server::hasClient()
        {
            return client && client.connected();
        }

        void Server::listen()
        {
            if (!client || !client.connected())
                client = srv.available();
        }

        String Server::readLine()
        {
            String line = "";
            unsigned long start = millis();
            while (millis() - start < 1000) // 1s timeout
            {
                while (client.available())
                {
                    char c = client.read();
                    if (c == '\n')
                        return line;
                    line += c;
                }
                delay(10); // avoid tight loop
            }
            return line;
        }
    }

    namespace udp
    {
        Socket socket;

        bool init(uint16_t port)
        {
            if (port == 0)
                port = socket.port;

            if (socket.begin(socket.port))
            {
                Serial.print(F("[UDP] LPort "));
                Serial.println(port);
                return true;
            }
            else
            {
                Serial.print(F("[UDP] Failed to start "));
                Serial.println(port);
                return false;
            }
        }

        // Via struct
        bool Socket::begin(uint16_t _port)
        {
            this->port = _port;
            return udp.begin(_port);
        }

    }

    namespace mdns
    {
        void init()
        {
            if (MDNS.begin(sys::config.host_name.c_str()))
            {
                MDNS.addService("http", "tcp", tcp::server.port);
                MDNS.addService("udp", "udp", udp::socket.port);
                Serial.print(F("[mDNS] Host-name: "));
                Serial.println(sys::config.host_name.c_str());
            }
            else
            {
                Serial.println(F("[mDNS] Failed to start"));
            }
        }
    }
}
