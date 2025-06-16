#include "../render/camera.h"

namespace s3gl
{
    class mesh
    {
    private:
        // vertex data
        std::size_t num_verts;
        std::vector<float> vert_data;
        std::vector<glm::vec3> m_verts;
        // texture data
        texture objTex;
        // vertex buffer objects
        VAO vao;
        VBO vbo;
        shader shad;
        // variables
        bool has_init;
        glm::mat4 model;
    public:
        glm::vec3 pos;
        mesh();
        mesh(const std::string& objfpath, const std::string& texfpath, const std::string& fragfpath, const std::string& vertfpath, int tex_unit, glm::vec3 pos);

        void link_atribute(std::vector<GLuint> layout_pos, std::vector<GLuint> num_comps, GLsizeiptr stride, std::vector<void*> offsets);

        void set_tex_flags(int preset);

        void set_tex_flags(GLuint pname, GLuint params);

        float get_height_data(glm::vec3 pos);

        void draw(camera& cam, const glm::vec3& light_pos, const glm::vec4& light_col, int light_preset);
    };
}