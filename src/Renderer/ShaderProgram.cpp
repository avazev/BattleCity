#include "ShaderProgram.h"

#include <iostream>

namespace Renderer {
    ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
        GLuint vertexShaderID;
        if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
        {
            std::cerr << "ERROR::VERTEX SHADER:: Compile time error\n" << std::endl;
            return;
        }

        GLuint fragmentShaderID;
        if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
        {
            std::cerr << "ERROR::FRAGMENT SHADER:: Compile time error\n" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertexShaderID);
        glAttachShader(ID, fragmentShaderID);
        glLinkProgram(ID);

        GLint success;
        glGetProgramiv(ID, GL_LINK_STATUS, &success);

        if (!success)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(ID, 1024, nullptr, infoLog); /**/
            std::cerr << "ERROR::SHADER PROGRAM::Link time error \n" << infoLog << std::endl;
        }
        else
        {
            m_isCompiled = true;
        }

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
	}

    bool ShaderProgram::createShader(const std::string& source, GLenum shaderType, GLuint& shaderID)
    {
        shaderID = glCreateShader(shaderType);
        const char* code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);

            std::cerr << "ERROR::SHADER::Compile time error \n" << infoLog << std::endl;
            return false;
        }

        return true;

    }
    
    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(ID);
    }

    void ShaderProgram::use() const
    {
        glUseProgram(ID);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
    {
        glDeleteProgram(ID);

        ID = shaderProgram.ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.ID = 0;
        shaderProgram.m_isCompiled = false;
        return *this;
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
    {
        ID = shaderProgram.ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.ID = 0;
        shaderProgram.m_isCompiled = false;
    }
}