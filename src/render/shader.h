#pragma once
#include "vao.h"

namespace s3gl
{
    class shader
    {
    private:
        bool inited;
        GLuint idFrag;
        GLuint idVert;
        std::string s_shader_frag;
        std::string s_shader_vert;
    public:
        GLuint id;

        shader();

        shader(const std::string& fpathVert, const std::string& fpathFrag);

        void attach();

        void activate();

        void free_shad();

        void free_prog();
    }; 
};