#include "scene.h"

s3gl::scene::scene(std::string_view fpath_json)
    : m_jsonFpath(fpath_json.data()), m_active(false), m_stage(1)
{
    std::ifstream file(m_jsonFpath.data());
    if (!file.is_open()) 
        throw s3gl::exception(std::string("Json filepath entered is invalid: " + std::string(m_jsonFpath.data())));
}

s3gl::mesh& s3gl::scene::get_mesh_ref(std::string_view name)
{
    return asset_manager::get_mesh(asset_manager::get_mesh_hash(name));
}

void s3gl::scene::load() 
{
    // Load JSON file
    std::ifstream file(m_jsonFpath.data());
    if (!file.is_open()) 
        throw s3gl::exception(std::string("Json filepath entered is invalid: " + std::string(m_jsonFpath.data())));

    nlohmann::json j;
    file >> j;

    std::cout << "[INFO]: Loading Scene: \"" << j["name"] << "\"\n";
    
    std::cout << "=== Loading shaders... ===\n";
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

    std::cout << "=== Loading textures... ===\n";
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
    
    std::cout << "=== Loading objects... ===\n";
    // Parse and load objects
    for (auto& [name, arr] : j["objects"].items())
    {
        std::string path = arr[0].get<std::string>();

        asset_manager::new_mesh(name.c_str(), 
                                arr[0].get<std::string>(),
                                asset_manager::get_shad_hash(arr[2].get<std::string_view>()),
                                asset_manager::get_tex_hash(arr[1].get<std::string_view>()),
                                glm::vec3(0.0f));
    }

    std::cout << "=== Loading audio... ===\n";
    // load audio buffers 
    for(auto& [name, fpath] : j["audio"].items())
    {
        s3gl::asset_manager::new_WAV_buffer(name.c_str(), fpath.get<std::string_view>());
    }
    // Parse and load sounds sounds actors
    for (auto& [name, arr] : j["events"]["sound_actors"].items())
    {
        std::string ab_name = arr[0].get<std::string>();
        
        std::size_t hash = 0;
        if (true || !arr[1].is_null()) // disabled null checking
        {
            std::string_view associated_Obj = arr[1].get<std::string>();
            hash = asset_manager::get_mesh_hash(associated_Obj);
        }

        std::size_t ab_hash = asset_manager::get_WAV_hash(ab_name);
        if(ab_hash == 0)
            throw "Audio given to sound actor does not exist\n";

        actor_manager::new_sound_actor(ab_hash, hash, name, glm::vec3(0.0f));
    }

    std::cout << "=== Getting stage and event info... ===\n";

    m_num_stages = j["number_of_stages"].get<size_t>();
    for(size_t i = 1; i <= m_num_stages; i++)
    {
        for(auto [key, pos] : j["stages"][std::to_string(i)]["objects"].items())
        {
            std::pair<std::string, glm::vec3> pair;
            pair.first = key;
            pair.second = glm::vec3(pos[0].get<float>(), pos[1].get<float>(), pos[2].get<float>());
            m_stage_mesh_list[i].push_back(pair);
        }

        for(auto [key, pos] : j["stages"][std::to_string(i)]["sound_actors"].items())
        {
            std::pair<std::string, glm::vec3> pair;
            pair.first = key;
            pair.second = glm::vec3(pos[0].get<float>(), pos[1].get<float>(), pos[2].get<float>());
            m_stage_sound_list[i].push_back(pair);
        }
    }

    std::cout << "=== Loading complete! ===\n";

    // flush the stream
    std::cout << std::endl;
}

bool s3gl::scene::operator()()
{
    return m_active;
}

void s3gl::scene::start()
{
    m_active = true;
}

void s3gl::scene::set_stage(size_t stage_num)
{
    if(stage_num <= m_num_stages && stage_num != m_stage)
    {
        actor_manager::stop_sounds();
        
        for(const auto& pair : m_stage_mesh_list.at(m_stage))
        {
            try
            {
                std::cout << pair.first << '\n';
                mesh& lmesh = asset_manager::get_mesh(asset_manager::get_mesh_hash(pair.first));
                lmesh.add_to_render_list = true;
                lmesh.pos = pair.second;
            }
            catch(s3gl::exception& e)
            {
                // for my sanity
                std::cout << e.what() << ' ' << pair.first << '\n';
                throw e;
            }
        }
        for(const auto& pair : m_stage_sound_list.at(m_stage))
        {
            try
            {
                std::cout << pair.first << '\n';
                sound_actor& lactor = actor_manager::get_actor(actor_manager::get_actor_hash(pair.first));
                lactor.m_play = true;
                lactor.m_pos = pair.second;
            }
            catch(s3gl::exception& e)
            {
                std::cout << e.what() << ' ' << pair.first << '\n';
                throw e;
            }
        }
        for(const auto& pair : m_stage_mesh_list.at(stage_num))
        {
            try
            {
                mesh& lmesh = asset_manager::get_mesh(asset_manager::get_mesh_hash(pair.first));
                lmesh.add_to_render_list = false;
            }
            catch(s3gl::exception& e)
            {
                std::cout << e.what() << ' ' << pair.first << '\n';
                throw e;
            }
        }
        for(const auto& pair : m_stage_sound_list.at(stage_num))
        {
            try
            {
                sound_actor& lactor = actor_manager::get_actor(actor_manager::get_actor_hash(pair.first));
                lactor.m_play = false;
            }
            catch(s3gl::exception& e)
            {
                std::cout << e.what() << ' ' << pair.first << '\n';
                throw e;
            }
        }
        m_stage = stage_num;
        actor_manager::queue_sound_events();
    }
}

void s3gl::scene::update()
{

}

void s3gl::scene::end() 
{
    actor_manager::stop_sounds();
    actor_manager::clear();
    asset_manager::clear();
    m_active = false;
}
