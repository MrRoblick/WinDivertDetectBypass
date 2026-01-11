#include <string_utils.h>

#include <string>
#include <random>

std::string string_utils::random_string(const size_t length) {
    std::string str;

    str.resize_and_overwrite(length, [](char* buffer, const size_t count) {
	    thread_local std::mt19937 rng(std::random_device{}());
	    thread_local std::uniform_int_distribution<> dist('a', 'z');

        for (size_t i = 0; i < count; ++i) {
            buffer[i] = static_cast<char>(dist(rng));
        }

        return count;
    });

    return str;
}