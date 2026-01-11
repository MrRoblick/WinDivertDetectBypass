#pragma once
#include <string_view>


namespace config {
    constexpr std::string_view name = "WinDivert Detect Bypass";
    constexpr std::string_view version = "1.0.0";
    constexpr std::string_view author = "MrRoblick";
    constexpr std::string_view year = "2026";

    constexpr std::string_view dll_name = "WinDivert.dll";
    constexpr std::string_view driver_name = "WinDivert64.sys";
}
