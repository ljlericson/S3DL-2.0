#include "shader.h"

s3gl::shader::shader()
{
    m_inited = false;
}

s3gl::shader::shader(std::string_view fpathVert, std::string_view fpathFrag)
{
    m_inited = false;
    std::ifstream filevert(fpathVert.data());
    std::ifstream filefrag(fpathFrag.data());
    
    if(!filevert || !filefrag)
    {
        std::cout << "[ERROR]: Invalid file given\n";
        throw exception("Invalid filepath given to shader upon init\n");
    }
    else
    {
        while(!filefrag.eof())
        {
            char c[1024];
            filefrag.getline(c, 1024);
            ms_shader_frag.append(c);
            ms_shader_frag.push_back('\n');
        }
        while(!filevert.eof())
        {
            char c[1024];
            filevert.getline(c, 1024);
            ms_shader_vert.append(c);
            ms_shader_vert.push_back('\n');
        }
        
        m_inited = true; 
    }
    
}

s3gl::shader::shader(const shader& other)
    : ms_shader_frag(other.ms_shader_frag), ms_shader_vert(other.ms_shader_vert), m_inited(other.m_inited)
{

}

void s3gl::shader::build()
{
    if(m_inited)
    {
        id = glCreateProgram();

        m_idFrag = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* shad_src = ms_shader_frag.c_str();
        glShaderSource(m_idFrag, 1, &shad_src, NULL);
        glCompileShader(m_idFrag);

        m_idVert = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* shad_src2 = ms_shader_vert.c_str();
        glShaderSource(m_idVert, 1, &shad_src2, NULL);
        glCompileShader(m_idVert);
    }
    else
    {   throw s3gl::exception("[ERROR]: Shader must be initialised before attempting to create\n");  }
}

void s3gl::shader::attach()
{
    if(m_inited)
    {
        glAttachShader(id, m_idFrag);
        glAttachShader(id, m_idVert);
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
    if(m_inited)
        glUseProgram(id);
}

void s3gl::shader::free_shad()
{
    if(m_inited)
    {
        GLint success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success) {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(m_idFrag);
        glDeleteShader(m_idVert);
    }
}

void s3gl::shader::free_prog()
{
    if(m_inited)
        glDeleteProgram(id);
}
