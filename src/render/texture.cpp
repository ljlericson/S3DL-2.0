#include "texture.h"

s3gl::texture::texture()
{

}

s3gl::texture::texture(const char* fpath, GLuint tex_unit, GLuint target)
{
    bytes = stbi_load(fpath, &t_width, &t_height, &num_col_chan, 0);
    if(!bytes)
    {
        std::cout << "[ERROR]: Could not load texture image \""<< fpath << "\", error: " << stbi_failure_reason() << "\n";
        return;
    }

    glGenTextures(1, &id);
    glActiveTexture(tex_unit);
    glBindTexture(target, id);
    this->target = target;

    GLenum format = (num_col_chan == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(target, 0, format, t_width, t_height, 0, format, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(target);

    stbi_image_free(bytes);  // free AFTER uploading
}

void s3gl::texture::set_param(GLuint pname, GLuint params)
{
    glTexParameteri(target, pname, params);
}

void s3gl::texture::bind(GLuint shad_program)
{
    glUseProgram(shad_program);
    GLuint tex0Uni = glGetUniformLocation(shad_program, "tex0");
    glUniform1i(tex0Uni, 0);
    if (tex0Uni == -1) std::cerr << "Uniform 'tex0' not found!" << std::endl;
}

void s3gl::texture::bind()
{
    glBindTexture(target, id);
}

void s3gl::texture::_delete()
{
    glDeleteTextures(1, &id);
}
