#pragma once

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader
{
private:
    void checkCompileErrors(GLuint shader, std::string type);

public:
    unsigned int ID;

    Shader(std::string vertexShaderPath, std::string fragmentShaderPath);

    void use();

    void setInt(const std::string& name, int value) const;

    void setMat4(const std::string& name, const glm::mat4& mat) const;

	void setVec4(const std::string& name, const glm::fvec4 color) const;

    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
};