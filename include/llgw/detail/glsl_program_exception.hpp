#if !defined(LLGW_INCLUDE_INTERNAL_HEADERS)
#if defined(_MSC_VER)
#pragma message("glsl_program_exception.hpp is an internal header file and \
must not be used directly. Please use llgw.hpp instead.")
#else
#warning "glsl_program_exception.hpp is an internal header file and must not \
be used directly. Please use llgw.hpp instead."
#endif
#define LLGW_INCLUDE_INTERNAL_HEADERS
#endif

#ifndef LLGW_GLSL_PROGRAM_EXCEPTION_H
#define LLGW_GLSL_PROGRAM_EXCEPTION_H

#include <string>
#include <exception>

namespace LLGW {

class GLSLProgramException : public std::exception {
public:
    GLSLProgramException(const std::string &description);
    const char *what(void) const noexcept;
private:
    std::string m_description;
}; // End of class


inline GLSLProgramException::GLSLProgramException(const std::string &description)
    : m_description(description)
{}

inline const char *GLSLProgramException::what(void) const noexcept
{
    return m_description.c_str();
}

} // namespace LLGW

#endif // LLGW_GLSL_PROGRAM_EXCEPTION_H
