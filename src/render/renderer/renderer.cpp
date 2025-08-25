#include "renderer.h"

void s3gl::renderer::render(s3gl::camera &cam, const glm::vec3 &light_pos, const glm::vec4 &light_col, int light_preset)
{
    for(const auto& [key, mesh] : asset_manager::get_mesh_map())
    {
        if(mesh->add_to_render_list)
            mesh->draw(cam, light_pos, light_col, light_preset);
    }
}