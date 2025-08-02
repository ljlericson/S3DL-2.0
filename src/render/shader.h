#pragma once
#include "vao.h"

namespace s3gl
{
    class shader
    {
    private:
        bool m_inited;
        GLuint m_idFrag;
        GLuint m_idVert;
        std::string ms_shader_frag;
        std::string ms_shader_vert;
    public:
        GLuint id;

        shader();
        shader(std::string_view fpathVert, std::string_view fpathFrag);

        shader(const shader& other);

        void build();

        void attach();

        void activate();

        void free_shad();

        void free_prog();
    }; 
};