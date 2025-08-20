#include "scene.h"

s3gl::scene::scene(std::string_view fpath_json)
{
    // Load JSON file
    std::ifstream file(fpath_json.data());
    if (!file.is_open()) 
        throw s3gl::exception(std::string("Json filepath entered is invalid: " + std::string(fpath_json.data())));

    nlohmann::json j;
    file >> j;

    // Parse and load shaders
    for (auto& [name, arr] : j["shaders"].items())
    {
        // entering as c_str() because
        // conversion between c_str and
        // strview is faster than str
        // to strview
        asset_manager::new_shad(name.c_str(), 
                                arr[0].get<std::string>().c_str(), 
                                arr[1].get<std::string>().c_str());
    }

    // Parse and load textures
    for (auto& [name, arr] : j["textures"].items())
    {
        // keeping order for a reason:
        // textures need shaders for
        // loading, objects neeed 
        // both shaders and textures
        // for loading
        asset_manager::new_tex(name.c_str(),
                               arr[1].get<std::string_view>(),
                               asset_manager::get_shad_hash(arr[0].get<std::string_view>()));
    }

    // Parse and load objects
    for (auto& [name, arr] : j["objects"].items())
    {
        std::string path = arr[0].get<std::string>();
        auto coords = arr[3];
        glm::vec3 pos(coords[0].get<float>(), coords[1].get<float>(), coords[2].get<float>());

        asset_manager::new_mesh(name.c_str(), 
                                arr[0].get<std::string>(),
                                asset_manager::get_shad_hash(arr[2].get<std::string_view>()),
                                asset_manager::get_tex_hash(arr[1].get<std::string_view>()),
                                pos);

        m_mesh_hash_map[name] = asset_manager::get_mesh_hash(name);
    }

    // Parse and load sounds sounds
    for (auto& [name, arr] : j["events"]["sound"].items())
    {
        std::string path = arr[0].get<std::string>();
        auto coords = arr[1];
        glm::vec3 pos(coords[0].get<float>(), coords[1].get<float>(), coords[2].get<float>());
        
        std::size_t hash = 0;
        if (!arr[2].is_null()) 
        {
            std::string_view associated_Obj = arr[2].get<std::string>();
            hash = asset_manager::get_mesh_hash(associated_Obj);
        }

        actor_manager::new_sound_actor(asset_manager::new_WAV_buffer(name, path), hash, pos);
    }

}

s3gl::mesh& s3gl::scene::get_mesh_ref(std::string_view name)
{
    return asset_manager::get_mesh(m_mesh_hash_map[name.data()]);
}