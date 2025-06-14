#include "texture.h"

s3gl::texture::texture() {}

s3gl::texture::texture(const char* fpath, GLuint tex_unit, GLuint target)
{
    bytes = stbi_load(fpath, &t_width, &t_height, &num_col_chan, 0);
    if(!bytes)
    {
        std::cout << "[ERROR]: Could not load texture image \"" << fpath << "\", error: " << stbi_failure_reason() << "\n";
    }
    else
    {
        this->tex_unit = tex_unit;
        this->target = target;

        glGenTextures(1, &id);
        glActiveTexture(GL_TEXTURE0 + tex_unit);
        glBindTexture(target, id);

        GLenum format = (num_col_chan == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(target, 0, format, t_width, t_height, 0, format, GL_UNSIGNED_BYTE, bytes);
        glGenerateMipmap(target);
        stbi_image_free(bytes);
    }
}

void s3gl::texture::set_param(GLuint pname, GLuint params)
{
    glBindTexture(target, id); // ensure texture is bound before setting parameters
    glTexParameteri(target, pname, params);
}

void s3gl::texture::bind(GLuint shad_program)
{
    glUseProgram(shad_program);
    glActiveTexture(GL_TEXTURE0 + tex_unit);
    glBindTexture(target, id);

    GLint loc = glGetUniformLocation(shad_program, "tex0");
    if(loc == -1)
    {
        std::cerr << "[ERROR]: Uniform not found!\n";
    }
    else
    {
        glUniform1i(loc, tex_unit);
    }
}

void s3gl::texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + tex_unit);
    glBindTexture(target, id);
}

void s3gl::texture::_delete()
{
    glDeleteTextures(1, &id);
}
