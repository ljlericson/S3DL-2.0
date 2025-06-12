#pragma once
#include "../s3gl/s3gl.h"

namespace s3gl
{
    class EBO
    {
    public:
        GLuint ID;

        EBO();
        EBO(GLuint* indices, GLsizeiptr size);

        void bind();
        void unbind();
        void free();
    };
};
