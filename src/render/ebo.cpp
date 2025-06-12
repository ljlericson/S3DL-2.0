#include "ebo.h"

// class EBO

s3gl::EBO::EBO()
{
    
}

s3gl::EBO::EBO(GLuint* indices, GLsizeiptr size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void s3gl::EBO::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void s3gl::EBO::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void s3gl::EBO::free()
{   
    glDeleteBuffers(1, &ID);
}
