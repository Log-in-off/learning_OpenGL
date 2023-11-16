#pragma once
#include <string>
#include <unordered_map>
#include <string_view>
#include <glm/glm.hpp>

namespace sh
{

class Shader
{
public:
    Shader(const char* shaderPath, const char* fragmentPath, const char* geometryPath = nullptr);
    ~Shader();
    Shader(const Shader &rhs) = delete;
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat2(const std::string& name, const glm::mat2& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
private:
    enum class TypeChecking
    {
        kVertexShader,
        kFragmentShader,
        kGeometryShader,
        kProgram
    };
    void checkCompileErrors(unsigned int shader, TypeChecking type);
    std::unordered_map<TypeChecking, std::string_view> names;
    unsigned int id_;
};

}