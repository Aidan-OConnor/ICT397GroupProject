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
};