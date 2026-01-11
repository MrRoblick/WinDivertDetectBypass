#include <console.h>
#include <print>
#include <Windows.h>

#include "color.h"
#include "config.h"


void console::setup_console() {
    const auto h_out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dw_mode = 0;
    GetConsoleMode(h_out, &dw_mode);
    dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h_out, dw_mode);
    SetConsoleOutputCP(CP_UTF8);
}

void console::print_banner()
{
    std::print("\033[2J\033[H");
    constexpr auto art = R"(
  _    _  _        ______  _                    _ 
 | |  | |(_)       |  _  \(_)                  | |
 | |  | | _  _ __  | | | | _ __   __ ___  _ __ | |_ 
 | |/\| || || '_ \ | | | || |\ \ / // _ \| '__|| __|
 \  /\  /| || | | || |/ / | | \ V /|  __/| |   | |_ 
  \/  \/ |_||_| |_||___/  |_|  \_/  \___||_|    \__|
                                                    
    )";

    std::println("{}{}{}", color::cyan, art, color::reset);

    std::println("{}======================================================{}", color::bold, color::reset);
    std::println(" {}{} v{}{}", color::green, config::name, config::version, color::reset);
    std::println(" {}Developed by {} (c) {}{}", color::magenta, config::author, config::year, color::reset);
    std::println("{}======================================================{}", color::bold, color::reset);
    std::println("");
}

void console::pause()
{
#ifdef _WIN32
    system("pause");
#elif __linux__
    system("read -p \"Press Enter to continue...\"");
#endif
}
