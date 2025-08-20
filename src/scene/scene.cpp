#include "scene.h"

s3gl::scene::scene(std::string_view fpath_json)
{
    // Load JSON file
    std::ifstream file(fpath_json.data());
    if (!file.is_open()) 
        throw s3gl::exception(std::string("Json filepath entered is invalid: " + std::string(fpath_json.data())));

    nlohmann::json j;
    file >> j;

    // Parse objects
    for (auto& [name, arr] : j["objects"].items())
    {
        std::string path = arr[0].get<std::string>();
        auto coords = arr[1];
        glm::vec3 pos(coords[0].get<float>(), coords[1].get<float>(), coords[2].get<float>());

        std::cout << "Object '" << name << "' from " << path
                  << " at (" << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
    }

    // Parse sounds
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