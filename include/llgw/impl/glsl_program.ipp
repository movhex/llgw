#if !defined(LLGW_INCLUDE_INTERNAL_HEADERS)
#if defined(_MSC_VER)
#pragma message("glsl_program.ipp is an internal header file and must \
not be used directly. Please use llgw.hpp instead.")
#else
#warning "glsl_program.ipp is an internal header file and must not be \
used directly. Please use llgw.hpp instead."
#endif
#define LLGW_INCLUDE_INTERNAL_HEADERS
#endif

#ifndef LLGW_IMPL_GLSL_PROGRAM_H
#define LLGW_IMPL_GLSL_PROGRAM_H

namespace LLGW {

LLGW_DECL const std::map<std::string, GLSLProgram::Type>
GLSLProgram::m_extensions_list = {
    {".vs",   GLSLProgram::Type::VERTEX},
    {".vert", GLSLProgram::Type::VERTEX},
    {".gs",   GLSLProgram::Type::GEOMETRY},
    {".geom", GLSLProgram::Type::GEOMETRY},
    {".tcs",  GLSLProgram::Type::TESS_CONTROL},
    {".tes",  GLSLProgram::Type::TESS_EVALUATION},
    {".fs",   GLSLProgram::Type::FRAGMENT},
    {".frag", GLSLProgram::Type::FRAGMENT},
    {".cs",   GLSLProgram::Type::COMPUTE}
};

LLGW_DECL GLSLProgram::GLSLProgram()
    : m_program_handle(0)
    , m_is_linked(false)
{}

LLGW_DECL void GLSLProgram::compileShaderFromFile(const std::string &filename)
{
    compileShaderFromFile(filename, getTypeByExtension(getExtension(filename)));
}

LLGW_DECL void GLSLProgram::compileShaderFromFile(const std::string &filename,
    Type type)
{   
    if (m_program_handle == 0) {
        m_program_handle = glCreateProgram();
        if (!m_program_handle) {
            throw GLSLProgramException("Failed to create shader program");
        }
    }
    
    std::ifstream file;

    file.open(filename, std::fstream::in);

    if (!file.is_open()) {
        throw GLSLProgramException(format("Failed to open file '%s'",
            filename.c_str()));
    }

    std::string data_from_file((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    compileShaderFromSource(data_from_file, type, filename);
}

LLGW_DECL void GLSLProgram::compileShaderFromSource(const std::string &code,
    Type type, const std::string &filename)
{
    if (m_program_handle == 0) {
        m_program_handle = glCreateProgram();
        if (!m_program_handle) {
            throw GLSLProgramException("Failed to create shader program");
        }
    }

    GLuint shader_handle = glCreateShader(static_cast<GLenum>(type));

    const char *src_code = code.c_str();
    glShaderSource(shader_handle, 1, &src_code, nullptr);

    glCompileShader(shader_handle);

    GLint result;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint log_len;
        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 0) {
            std::string log_str(0, ' ');
            log_str.reserve(log_len);
            GLsizei written;
            glGetShaderInfoLog(shader_handle, log_len, &written,
                log_str.data());
            throw GLSLProgramException(
                format("Failed to compile shader: %s: %s", filename.c_str(), 
                    log_str.c_str()));
        }
    }
    else {
        glAttachShader(m_program_handle, shader_handle);
    }
}

LLGW_DECL void GLSLProgram::link(void)
{
    if (m_is_linked) {
        return;
    }
    if (m_program_handle <= 0) {
        throw GLSLProgramException("Program has not been compiled");
    }

    glLinkProgram(m_program_handle);

    GLint status = 0;
    glGetProgramiv(m_program_handle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_len;
        glGetProgramiv(m_program_handle, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 0) {
            std::string log_str(0, ' ');
            log_str.reserve(log_len);
            GLsizei written;
            glGetProgramInfoLog(m_program_handle, log_len, &written,
                log_str.data());
            throw GLSLProgramException(format("Failed to link program: %s",
                log_str.c_str()));
        }
    }
    else {
        findUniformLocations();
        m_is_linked = true;
    }
}

LLGW_DECL void GLSLProgram::use(void)
{
    if (m_program_handle == 0 || (!m_is_linked)) {
        throw GLSLProgramException("Shader has not been linked");
    }
    glUseProgram(m_program_handle);
}

LLGW_DECL void GLSLProgram::validate(void) const
{
    if (!m_is_linked) {
        throw GLSLProgramException("Program is not linked");
    }

    glValidateProgram(m_program_handle);

    std::string error_description;

    GLint status = 0;
    glGetProgramiv(m_program_handle, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_len;
        glGetProgramiv(m_program_handle, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 0) {
            std::string log_str(0, ' ');
            log_str.reserve(log_len);
            GLsizei written;
            glGetProgramInfoLog(m_program_handle, log_len, &written,
                log_str.data());
            error_description.assign(log_str);
        }
        throw GLSLProgramException(format("Failed to validate program: %s",
            error_description.c_str()));
    }
}

LLGW_DECL bool GLSLProgram::isLinked(void) const
{
    return m_is_linked;
}

LLGW_DECL GLuint GLSLProgram::getHandle(void) const
{
    return m_program_handle;
}

LLGW_DECL void GLSLProgram::bindAttribLocation(GLuint location,
    const std::string &name)
{
    glBindAttribLocation(m_program_handle, location, name.c_str());
}

LLGW_DECL void GLSLProgram::bindFragDataLocation(GLuint location,
    const std::string &name)
{
    glBindFragDataLocation(m_program_handle, location, name.c_str());
}

LLGW_DECL void GLSLProgram::setUniform(const std::string &name,
    float x, float y, float z)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniform3f(loc, x, y, z);
}

LLGW_DECL void GLSLProgram::setUniform(const std::string &name,
    const glm::vec3 &v)
{
    setUniform(name.c_str(), v.x, v.y, v.z);
}

LLGW_DECL void GLSLProgram::setUniform(const std::string &name,
    const glm::vec4 &v)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

LLGW_DECL void GLSLProgram::setUniform(const std::string &name,
    const glm::vec2 &v)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniform2f(loc, v.x, v.y);
}

LLGW_DECL void GLSLProgram::setUniform(const std::string &name,
    const glm::mat4 &m)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

LLGW_DECL void GLSLProgram::setUniform(const std::string &name,
    const glm::mat3 &m)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

LLGW_DECL void GLSLProgram::setUniform(const std::string &name, float val)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniform1f(loc, val);
}

LLGW_DECL void GLSLProgram::setUniform(const std::string &name, int val)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniform1i(loc, val);
}

LLGW_DECL void GLSLProgram::setUniform(const std::string &name, GLuint val)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniform1ui(loc, val);
}

LLGW_DECL void GLSLProgram::setUniform(const std::string &name, bool val)
{
    int loc = getUniformLocation(name.c_str());
    glUniform1i(loc, val);
}

LLGW_DECL void GLSLProgram::findUniformLocations(void)
{
    m_uniform_locations.clear();

    GLint n_uniforms = 0;

    // For OpenGL 4.1, use glGetActiveUniform
#if defined(__APPLE__)

    GLint max_len;

    glGetProgramiv(m_program_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_len);
    glGetProgramiv(m_program_handle, GL_ACTIVE_UNIFORMS, &n_uniforms);

    std::string name(0, ' ');
    name.reserve(max_len);

    for (GLuint i = 0; i < n_uniforms; ++i) {
        GLint size;
        GLenum type;
        GLsizei written;
        glGetActiveUniform(m_program_handle, i, max_len, &written, &size,
            &type, name.data());
        GLint location = glGetUniformLocation(m_program_handle, name.c_str());
        m_uniform_locations[name] = glGetUniformLocation(m_program_handle,
            name.c_str());
    }

    // For OpenGL 4.3 and above, use glGetProgramResource
#else

    glGetProgramInterfaceiv(m_program_handle, GL_UNIFORM, GL_ACTIVE_RESOURCES,
        &n_uniforms);

    GLenum properties[] = {
        GL_NAME_LENGTH,
        GL_TYPE,
        GL_LOCATION,
        GL_BLOCK_INDEX
    };

    for (GLint i = 0; i < n_uniforms; ++i) {

        GLint results[4];
        glGetProgramResourceiv(m_program_handle, GL_UNIFORM, i, 4, properties,
            4, nullptr, results);

        if (results[3] != -1) {
            // Skip uniforms in blocks
            continue;
        }
        GLint name_buf_size = results[0] + 1;

        std::string name(0, ' ');
        name.reserve(name_buf_size);

        glGetProgramResourceName(m_program_handle, GL_UNIFORM, i,
            name_buf_size, nullptr, name.data());
        m_uniform_locations[name] = results[2];
    }
#endif
}

LLGW_DECL std::string GLSLProgram::getExtension(const std::string &filename)
{
    size_t pos = filename.find_last_of('.');
    if (pos != std::string::npos) {
        return filename.substr(pos);
    }
    return "";
}

LLGW_DECL GLSLProgram::Type GLSLProgram::getTypeByExtension(
    const std::string &extension)
{
    auto pos = m_extensions_list.find(extension);
    if (pos == m_extensions_list.end()) {
        throw GLSLProgramException("Failed to determine file format");
    }
    return pos->second;
}

LLGW_DECL GLint GLSLProgram::getUniformLocation(const std::string &name)
{
    auto pos = m_uniform_locations.find(name);
    if (pos == m_uniform_locations.end()) {
        m_uniform_locations[name] = glGetUniformLocation(m_program_handle,
            name.c_str());
    }
    return m_uniform_locations[name];
}

LLGW_DECL GLSLProgram::~GLSLProgram(void)
{
    if (m_program_handle == 0) {
        return;
    }

    GLint n = 0;
    glGetProgramiv(m_program_handle, GL_ATTACHED_SHADERS, &n);

    std::vector<GLuint> shaders(n);
    glGetAttachedShaders(m_program_handle, n, nullptr, shaders.data());

    auto f = [] (GLuint shader) { glDeleteShader(shader); };

    std::for_each(shaders.begin(), shaders.end(), f);

    glDeleteProgram(m_program_handle);
}

} // namespace LLGW

#endif // LLGW_IMPL_GLSL_PROGRAM_H
