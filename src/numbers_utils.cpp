#include <numbers_utils.h>
#include <random>

size_t numbers_utils::random_size_t(const size_t min, const size_t max)
{
    thread_local std::mt19937_64 generator(std::random_device{}());
    std::uniform_int_distribution distribution(min, max);
    return distribution(generator);
}
