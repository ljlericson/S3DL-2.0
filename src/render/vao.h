#pragma once
#include "vbo.h"

namespace s3gl
{
    class VAO
    {
    public:
        GLuint ID;

        VAO();

        void link_attrib(VBO& vbo, GLuint layout, GLuint num_comp, GLenum type, GLsizeiptr stride, void* offset);
        void bind();
        void unbind();
        void free();
    };

};