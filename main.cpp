#include <iostream>
#include <print>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <random>
#include <filesystem>

#include <color.h>
#include <config.h>
#include <errors.h>

#include <fs_utils.h>
#include <string_utils.h>
#include <offsets.h>

namespace fs = std::filesystem;

static void setup_console() {
    const auto h_out = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dw_mode = 0;
    GetConsoleMode(h_out, &dw_mode);
    dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h_out, dw_mode);
    SetConsoleOutputCP(CP_UTF8);
}

static void print_banner()
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

static size_t random_size_t(size_t min, size_t max)
{
	thread_local std::mt19937_64 generator(std::random_device{}());
    std::uniform_int_distribution<size_t> distribution(4, config::driver_name.size());
    return distribution(generator);
}

int main()
{
    setup_console();
    print_banner();

    std::println("Enter The Folder Path to WinDivert");

    std::string path;
    std::cin >> path;

    std::println("");

    const auto win_divert_dll_path = fs_utils::find_file(path, config::dll_name);
    if (!win_divert_dll_path)
    {
        std::println(stderr, "{}", win_divert_dll_path.error());
        return static_cast<int>(errors::failed_to_find_file);
    }

	std::ifstream win_divert_dll_file(win_divert_dll_path.value(), std::ios::binary);

    if (!win_divert_dll_file.is_open()) {
        std::println(stderr, "Failed to open file: {}", win_divert_dll_path.value().string());
        return static_cast<int>(errors::failed_to_open_file);
    }

    win_divert_dll_file.seekg(offsets::win_divert_dll_driver_name, std::ios::beg);
    if (!win_divert_dll_file) {
        std::println(stderr, "Offset error(offset out of file limit ? )");
        return static_cast<int>(errors::failed_to_file_offset);
    }

    std::wstring f_driver_name;
    f_driver_name.resize(config::driver_name.size());

    win_divert_dll_file.read(reinterpret_cast<char*>(f_driver_name.data()), config::driver_name.size() * sizeof(wchar_t));
    win_divert_dll_file.close();


    std::println("{}File driver name: {}{}", color::magenta, std::filesystem::path(f_driver_name).string(), color::reset);

    const auto gen_name = string_utils::random_string(random_size_t(0, config::driver_name.size()));
    const auto root_path = win_divert_dll_path.value().root_path();
    const auto parent_dir = win_divert_dll_path.value().parent_path();

    try
    {
        fs::rename(parent_dir / f_driver_name, parent_dir / gen_name);
    } catch (const fs::filesystem_error& e)
    {
        std::println(stderr, "Failed to rename {}", e.what());
        return static_cast<int>(errors::failed_to_rename_file);
    }

    std::fstream patcher(win_divert_dll_path.value(), std::ios::binary | std::ios::in | std::ios::out);

    if (!patcher.is_open()) {
        std::println(stderr, "Failed to open file for patching!");
        return static_cast<int>(errors::failed_to_open_file);
    }

    std::wstring new_driver_wname = fs::path(gen_name).wstring();
	patcher.seekp(offsets::win_divert_dll_driver_name, std::ios::beg);

    size_t max_chars = config::driver_name.size();
    size_t write_chars = new_driver_wname.size();

    if (write_chars > max_chars) {
        write_chars = max_chars;
        new_driver_wname.resize(max_chars);
        std::println(stderr, "Warning: New name is too long, truncated.");
    }
	patcher.write(reinterpret_cast<const char*>(new_driver_wname.data()), write_chars * sizeof(wchar_t));

    if (write_chars < max_chars) {
        size_t diff = max_chars - write_chars;
        std::vector<char> zeros(diff * sizeof(wchar_t), 0);
        patcher.write(zeros.data(), zeros.size());
    }

    patcher.close();
    std::println("{}Successfully patched DLL at offset 0x{:X} with '{}'{}", color::green, offsets::win_divert_dll_driver_name, gen_name, color::reset);

	return 0;
}
