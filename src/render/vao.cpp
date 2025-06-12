#include "vao.h"

// class VAO

s3gl::VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

void s3gl::VAO::link_attrib(VBO& vbo, GLuint layout, GLuint num_comp, GLenum type, GLsizeiptr stride, void* offset)
{
    vbo.bind();
    glVertexAttribPointer(layout, num_comp, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    vbo.unbind();
}

void s3gl::VAO::bind()
{
    glBindVertexArray(ID);
}

void s3gl::VAO::unbind()
{
    glBindVertexArray(0);
}

void s3gl::VAO::free()
{   
    glDeleteVertexArrays(1, &ID);
}