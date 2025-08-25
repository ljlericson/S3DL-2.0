#include "../../scene/scene.h"

namespace s3gl
{
    class renderer
    {
    private:
        std::vector<std::size_t> mesh_hashes;
        
    public:
        renderer() = default;
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;

        
        static void render(s3gl::camera &cam, const glm::vec3 &light_pos, const glm::vec4 &light_col, int light_preset);
    };
}