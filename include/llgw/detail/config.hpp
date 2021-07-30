#if !defined(LLGW_INCLUDE_INTERNAL_HEADERS)
#if defined(_MSC_VER)
#pragma message("config.hpp is an internal header file and must \
not be used directly. Please use llgw.hpp instead.")
#else
#warning "config.hpp is an internal header file and must not be \
used directly. Please use llgw.hpp instead."
#endif
#define LLGW_INCLUDE_INTERNAL_HEADERS
#endif

#ifndef LLGW_DETAIL_CONFIG_HPP
#define LLGW_DETAIL_CONFIG_HPP

// Default to a header-only implementation. The user must specifically request
// separate compilation by defining either LLGW_SEPARATE_COMPILATION.
#if !defined(LLGW_HEADER_ONLY)
#if !defined(LLGW_SEPARATE_COMPILATION)
#define LLGW_HEADER_ONLY 1
#endif
#endif

#if defined(LLGW_HEADER_ONLY)
#define LLGW_DECL  inline
#else
#define LLGW_DECL
#endif

#endif // LLGW_DETAIL_CONFIG_HPP
