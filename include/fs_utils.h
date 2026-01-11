#pragma once
#include <expected>
#include <string_view>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace fs_utils
{
    std::expected<fs::path, std::string> find_file(const fs::path& root_dir, std::string_view target_filename);
}