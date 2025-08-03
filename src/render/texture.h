#pragma once
#include "shader.h"

namespace s3gl
{
    class texture
    {
    private:
        // weird opengl stuff
        GLuint id;
        GLuint target;
        GLuint tex_unit;
        // texture info
        int t_width, 
        t_height, 
        num_col_chan;
        // texture bytes
        uint8_t* bytes;
        
    public:
        texture();

        ~texture();

        texture(const char* fpath, GLuint tex_unit, GLuint target);

        void set_param(GLuint pname, GLuint params);

        void bind(GLuint shad_program);

        void bind();

        void _delete();
    };
};