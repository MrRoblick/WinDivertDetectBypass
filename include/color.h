#pragma once
#include <string_view>

namespace color {
    constexpr std::string_view reset = "\033[0m";
    constexpr std::string_view red = "\033[31m";
    constexpr std::string_view green = "\033[32m";
    constexpr std::string_view yellow = "\033[33m";
    constexpr std::string_view cyan = "\033[36m";
    constexpr std::string_view bold = "\033[1m";
    constexpr std::string_view magenta = "\033[35m";
}