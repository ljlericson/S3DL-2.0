#include "vbo.h"

s3gl::VBO::VBO()
{
    
}

s3gl::VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void s3gl::VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void s3gl::VBO::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void s3gl::VBO::free()
{   
    glDeleteBuffers(1, &ID);
}