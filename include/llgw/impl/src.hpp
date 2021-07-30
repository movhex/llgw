#if !defined(LLGW_SEPARATE_COMPILATION)
#if defined(_MSC_VER)
#pragma message("Do not compile LLGW source without LLGW_SEPARATE_COMPILATION \
definition.")
#else
#warning "Do not compile LLGW source without LLGW_SEPARATE_COMPILATION \
definition."
#endif

#ifndef LLGW_IMPL_SRC_HPP
#define LLGW_IMPL_SRC_HPP

#include "llgw/impl/glsl_program.ipp"

#endif // LLGW_IMPL_SRC_HPP
