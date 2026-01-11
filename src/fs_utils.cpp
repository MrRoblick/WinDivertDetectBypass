#include <fs_utils.h>
#include <expected>

#include <string_view>
#include <string>

namespace fs = std::filesystem;

std::expected<fs::path, std::string> fs_utils::find_file(const fs::path& root_dir, const std::string_view target_filename) {
    if (!fs::exists(root_dir) || !fs::is_directory(root_dir)) {
        return std::unexpected(std::format("Error: Directory does not exist or path is invalid: {}", root_dir.string()));
    }

    try {
        for (const auto& entry : fs::recursive_directory_iterator(root_dir)) {
            try {
                if (entry.is_regular_file() && entry.path().filename() == target_filename) {
                    return entry.path();
                }
            }
            catch (const fs::filesystem_error& e) {
                return std::unexpected(std::format("Error accessing file entry: {}", e.what()));
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        return std::unexpected(std::format("Directory iteration error: {}", e.what()));
    }
    return std::unexpected(std::format("File \"{}\" not found", target_filename));
}