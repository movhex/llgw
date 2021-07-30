#if !defined(LLGW_INCLUDE_INTERNAL_HEADERS)
#if defined(_MSC_VER)
#pragma message("string_util.hpp is an internal header file and must \
not be used directly. Please use llgw.hpp instead.")
#else
#warning "string_util.hpp is an internal header file and must not be \
used directly. Please use llgw.hpp instead."
#endif
#define LLGW_INCLUDE_INTERNAL_HEADERS
#endif

#ifndef LLGW_STRING_UTIL_HPP
#define LLGW_STRING_UTIL_HPP

#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <string>
#include <memory>
#include <stdexcept>

namespace LLGW {

template <typename ... Args>
std::string format(const std::string& format, Args ... args)
{
    int len = std::snprintf(nullptr, 0, format.c_str(), args ... ) + 1;
    if (len <= 0) {
        throw std::runtime_error("Failed to format string");
    }

    size_t size = static_cast<size_t>(len);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);
}

} // namespace LLGW

#endif // LLGW_STRING_UTIL_HPP
