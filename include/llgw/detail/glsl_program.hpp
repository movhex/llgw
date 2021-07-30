#if !defined(LLGW_INCLUDE_INTERNAL_HEADERS)
#if defined(_MSC_VER)
#pragma message("glsl_program.h is an internal header file and must \
not be used directly. Please use llgw.hpp instead.")
#else
#warning "glsl_program.h is an internal header file and must not be \
used directly. Please use llgw.hpp instead."
#endif
#define LLGW_INCLUDE_INTERNAL_HEADERS
#endif

#ifndef LLGW_GLSL_PROGRAM_HPP
#define LLGW_GLSL_PROGRAM_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <glm/glm.hpp>

#include "llgw/detail/string_util.hpp"
#include "llgw/detail/glsl_program_exception.hpp"

namespace LLGW {

class GLSLProgram {
public:
    enum class Type;
public:
    GLSLProgram(void);
    GLSLProgram(const GLSLProgram &other) = delete;
    GLSLProgram(GLSLProgram &&other) = delete;
    void operator=(const GLSLProgram &other) = delete;
    void operator=(GLSLProgram &&other) = delete;
    void compileShaderFromFile(const std::string &filename);
    void compileShaderFromFile(const std::string &filename, Type type);
    void compileShaderFromSource(const std::string &code, Type type, const std::string &filename = "");
    void link(void);
    void use(void);
    void validate(void) const;
    bool isLinked(void) const;
    GLuint getHandle(void) const;
    void bindAttribLocation(GLuint location, const std::string &name);
    void bindFragDataLocation(GLuint location, const std::string &name);
    void setUniform(const std::string &name, float x, float y, float z);
    void setUniform(const std::string &name, const glm::vec2 &v);
    void setUniform(const std::string &name, const glm::vec3 &v);
    void setUniform(const std::string &name, const glm::vec4 &v);
    void setUniform(const std::string &name, const glm::mat4 &m);
    void setUniform(const std::string &name, const glm::mat3 &m);
    void setUniform(const std::string &name, float val);
    void setUniform(const std::string &name, int val);
    void setUniform(const std::string &name, bool val);
    void setUniform(const std::string &name, GLuint val);
    void findUniformLocations(void);
    ~GLSLProgram(void);
private:
    std::string getExtension(const std::string &filename);
    Type getTypeByExtension(const std::string &extension);
    GLint getUniformLocation(const std::string &name);
private:
    GLuint m_program_handle;
    bool m_is_linked;
    static const std::map<std::string, Type> m_extensions_list;
    std::map<std::string, GLint> m_uniform_locations;
}; // End of class


enum class GLSLProgram::Type {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    TESS_CONTROL = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
    COMPUTE = GL_COMPUTE_SHADER
};

} // namespace LLGW

#if defined(LLGW_HEADER_ONLY)
#include "llgw/impl/glsl_program.ipp"
#endif

#endif // LLGW_GLSL_PROGRAM_HPP
