#include "renderer.h"

void s3gl::renderer::render(s3gl::camera &cam, const glm::vec3 &light_pos, const glm::vec4 &light_col, int light_preset)
{
    for(const auto& mesh : asset_manager::get_mesh_map())
    {
        mesh.second->draw(cam, light_pos, light_col, light_preset);
    }
}