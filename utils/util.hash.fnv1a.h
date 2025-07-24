#pragma once

#include <stdint.h>
#include <stddef.h>

// Util Hash
inline constexpr uint32_t FNV_OFFSET = 2166136261u;
inline constexpr uint32_t FNV_PRIME = 16777619u;

inline constexpr uint32_t fnv1a(const char* str, size_t idx = 0, uint32_t hash = FNV_OFFSET) {
    return str[idx] == '\0' ? hash : fnv1a(str, idx + 1, (hash ^ str[idx]) * FNV_PRIME);
}
