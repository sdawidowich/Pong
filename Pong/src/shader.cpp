#include "shader.h"
#include "renderer.h"

#include <iostream>
#include <GL/glew.h>
#include <sstream>
#include <string>
#include <fstream>

Shader::Shader(const std::string& filepath)
    : m_filepath(filepath), m_rendererID(0) {

    // Gets shader sources from shader file, creates the shaders, and compiles the shader program
    ProgramShaderSource sources = parseShader(filepath);
    m_rendererID = createShader(sources.vertex_shader, sources.fragment_shader);
}

Shader::~Shader() {
    glCall(glDeleteProgram(m_rendererID));
}

void Shader::bind() const {
    glCall(glUseProgram(m_rendererID));
}

void Shader::unbind() const {
    glCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value) {
    glCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    glCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

ProgramShaderSource Shader::parseShader(const std::string& filepath) {
    // Open file
    std::ifstream stream(filepath);

    // Define shader types as enums
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    // Parses shader file line by line 
    while (getline(stream, line)) {
        // Update current shader type if line has #shader tag, otherwise write line data into shader source array
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    glCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    glCall(glShaderSource(id, 1, &src, nullptr));
    glCall(glCompileShader(id));

    // Shader error catching
    int result;
    glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) {
        int length;
        glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        glCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

int Shader::createShader(const std::string& vertex_shader, const std::string& fragment_shader) {
    glCall(unsigned int program = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

    glCall(glAttachShader(program, vs));
    glCall(glAttachShader(program, fs));
    glCall(glLinkProgram(program));
    glCall(glValidateProgram(program));

    glCall(glDeleteShader(vs));
    glCall(glDeleteShader(fs));

    return program;
}

int Shader::getUniformLocation(const std::string& name) {
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    glCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    if (location == -1)
        std::cout << "[Warning]: Uniform '" << name << "' doesn't exist!" << std::endl;
    m_uniformLocationCache[name] = location;
    return location;
}
