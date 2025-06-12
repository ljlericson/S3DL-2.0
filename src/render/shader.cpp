#include "shader.h"

s3gl::shader::shader()
{
    inited = false;
}

s3gl::shader::shader(const std::string& fpathVert, const std::string& fpathFrag)
{
    inited = false;
    std::ifstream file(fpathVert);
    std::ifstream filefrag(fpathFrag);
    
    if(!file || !filefrag)
    {
        std::cout << "[ERROR]: Invalid file given\n";
        throw exception("Invalid filepath given to shader upon init\n");
    }
    else
    {
        id = glCreateProgram();

        while(!filefrag.eof())
        {
            char c[1024];
            filefrag.getline(c, 1024);
            s_shader_frag.append(c);
            s_shader_frag.push_back('\n');
        }
        idFrag = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* shad_src = s_shader_frag.c_str();
        glShaderSource(idFrag, 1, &shad_src, NULL);
        glCompileShader(idFrag);
        while(!file.eof())
        {
            char c[1024];
            file.getline(c, 1024);
            s_shader_vert.append(c);
            s_shader_vert.push_back('\n');
        }
        idVert = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* shad_src2 = s_shader_vert.c_str();
        glShaderSource(idVert, 1, &shad_src2, NULL);
        glCompileShader(idVert);
        inited = true; 
    }
    
}

void s3gl::shader::attach()
{
    if(inited)
    {
        glAttachShader(id, idFrag);
        glAttachShader(id, idVert);
        glLinkProgram(id);

        GLint success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "[ERROR]: Shader Program Linking Failed\n" << infoLog << "\n";
        }
    }
    else
    {
        std::cout << "[ERROR]: Shader has not been inited\n";
    }
}

void s3gl::shader::activate()
{
    if(inited)
        glUseProgram(id);
}

void s3gl::shader::free_shad()
{
    if(inited)
    {
        GLint success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success) {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(idFrag);
        glDeleteShader(idVert);
    }
}

void s3gl::shader::free_prog()
{
    if(inited)
        glDeleteProgram(id);
}
