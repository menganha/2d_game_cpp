#pragma once

#include <cstddef>
#include <cstring>

template<std::size_t capacity>
class FString
{
public:
    constexpr FString() = default;
    constexpr FString(const char* str) : m_size{min(strlen(str), capacity)} { strncpy(m_string, str, m_size); }
    constexpr std::size_t Length() { return m_size; }

private:
    constexpr std::size_t min(std::size_t a, std::size_t b) { return (b < a) ? b : a; }
    char                  m_string[capacity + 1]{};
    std::size_t           m_size{};
};

using FString8 = FString<7>;
using FString16 = FString<15>;
using FString32 = FString<32>;
using FString64 = FString<63>;

