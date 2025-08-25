#include "../render/camera.h"

namespace s3gl
{
    class mesh
    {
    private:
        // vertex data
        std::size_t m_num_verts;
        std::vector<glm::vec3> m_verts;
        // texture data
        texture m_tex;
        // vertex buffer objects
        VAO m_vao;
        VBO m_vbo;
        shader m_shad;
        // variables
        bool m_init;
        glm::mat4 m_model;
    public:
        bool add_to_render_list; // sneaky reference to s3dl here
        glm::vec3 pos;
        mesh();
        mesh(const std::string& objfpath, const s3gl::shader& shad, s3gl::texture& tex, glm::vec3 pos);

        void link_atribute(const std::vector<GLuint>& layout_pos, const std::vector<GLuint>& num_comps, GLsizeiptr stride, const std::vector<void*>& offsets);

        void set_tex_flags(int preset);

        void set_tex_flags(GLuint pname, GLuint params);

        float get_height_data(const glm::vec3& pos);

        void draw(camera& cam, const glm::vec3& light_pos, const glm::vec4& light_col, int light_preset);
    };
}