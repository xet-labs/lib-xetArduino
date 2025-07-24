#include "utils.h"

namespace lgc
{
    static std::unordered_map<std::string, LogicCore> cores;

    // Initially assign default core1
    LogicCore core;

    void assign(const std::string& label)
    {
        auto it = cores.find(label);
        if (it != cores.end())
        {
            core = it->second;
            Serial.print("[lgc] Assigned core: ");
            Serial.println(label.c_str());
        }
        else
        {
            Serial.print("[lgc] Unknown core: ");
            Serial.println(label.c_str());
        }
    }
    void list()
    {
        Serial.println("[lgc] Available cores:");
        for (const auto& [label, logic] : cores)
        {
            Serial.print(" - ");
            Serial.println(label.c_str());
        }
    }
    const std::string& current()
    {
        return core.label;
    }

    void add(std::function<void()> fn, const std::string& label)
    {
        cores[label] = { fn, label };
    }
}
