#pragma once

#include <stdint.h>
#include <vector>
#include <span>
#include <string>
#include <expected>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <unordered_map>
#include <functional>

// Error are the negative CRC32 value of their name

#define MAKE_ERROR(x) (-(x))

enum class Errors : long
{
    FILE_NOT_FOUND = MAKE_ERROR(0x8F09F696L),
    INVALID_MAGIC = MAKE_ERROR(0xCB5BDACFL),
};

namespace Suitchi
{

template<typename T>
using result = std::expected<T, Errors>;

using error = std::unexpected<Errors>;

}

// KObject Handle
typedef uint32_t Handle;