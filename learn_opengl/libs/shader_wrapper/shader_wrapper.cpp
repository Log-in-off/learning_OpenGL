#include "shader_wrapper.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace sh {

Shader::~Shader()
{
    glDeleteProgram(id_);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    names[TypeChecking::kVertexShader] = "VERTEX";
    names[TypeChecking::kFragmentShader] = "FRAGMENT";
    names[TypeChecking::kGeometryShader] = "GEOMETRY";
    names[TypeChecking::kProgram] = "PROGRAM";

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if (nullptr != geometryPath)
        {
            std::stringstream gShaderStream;
            gShaderFile.open(geometryPath);
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderCode = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderCode ,nullptr);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, TypeChecking::kVertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderCode = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderCode ,nullptr);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, TypeChecking::kFragmentShader);

    unsigned int geometryShader;
    if (nullptr != geometryPath)
    {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        const char* geometryShaderCode = geometryCode.c_str();
        glShaderSource(geometryShader, 1, &geometryShaderCode ,nullptr);
        glCompileShader(geometryShader);
        checkCompileErrors(geometryShader, TypeChecking::kGeometryShader);
    }


    id_ = glCreateProgram();
    glAttachShader(id_, vertexShader);
    glAttachShader(id_, fragmentShader);
    if (nullptr != geometryPath)
        glAttachShader(id_, geometryShader);
    glLinkProgram(id_);
    checkCompileErrors(id_, TypeChecking::kProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(geometryPath != nullptr)
        glDeleteShader(geometryShader);

}

void Shader::use()
{
    glUseProgram(id_);
}

void Shader::checkCompileErrors(unsigned int shader, TypeChecking type)
{
    char infoLog[1024];
    int success;
    if (TypeChecking::kProgram != type)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            std::cout << "ERROR::SHADER:" << names[type] << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            std::cout << "ERROR::SHADER:" << names[type] << "::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat2(const std::string& name, const glm::mat2& value) const
{
    glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const
{
    glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

}