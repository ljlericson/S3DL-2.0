#pragma once
#include "ebo.h"

namespace s3gl
{
    class VBO
    {
    public:
        GLuint ID;

        VBO();
        VBO(GLfloat* vertices, GLsizeiptr size);

        void bind();
        void unbind();
        void free();
    };
};