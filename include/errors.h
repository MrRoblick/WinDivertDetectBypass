#pragma once
#include <cstdint>

enum class errors : uint8_t
{
	failed_to_find_file,
	failed_to_read_file,
	failed_to_open_file,
	failed_to_rename_file,
	failed_to_file_offset,
};
