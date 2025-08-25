#include "actor_manager.h"

void s3gl::actor_manager::init()
{
    m_device = alcOpenDevice(nullptr);
    if (!m_device) throw s3gl::exception("Failed to open audio device");
    
    m_context = alcCreateContext(m_device, nullptr);
    if (!m_context) throw s3gl::exception("Failed to create audio context");

    alcMakeContextCurrent(m_context);
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}

std::size_t s3gl::actor_manager::new_sound_actor(s3gl::asset_manager::audio_buffer_hash abhash, s3gl::asset_manager::mesh_hash mhash, std::string_view name, const glm::vec3& pos)
{
    std::size_t hash = s3gl::fnv1a_hash((void*)name.data(), name.size());
    sm_sound_key_map[std::string(name)] = hash;
    std::cout << sm_sound_key_map.at(std::string(name));
    
    sm_sound_actors[hash] = std::make_unique<sound_actor>(abhash, mhash, pos);
    return hash;
}

void s3gl::actor_manager::queue_sound_events()
{
    // sm_sound_actors[hash]->play_sound(m_device, m_context);
    for(const auto& pair : sm_sound_actors)
    {
        if(pair.second->m_play)
            pair.second->play_sound(m_device, m_context);
    }
}

void s3gl::actor_manager::stop_sounds()
{
    for(const auto& pair : sm_sound_actors)
    {
        pair.second->stop_sound();
    }
}

void s3gl::actor_manager::clear()
{
    sm_sound_actors.clear();
    sm_sound_key_map.clear();
}

s3gl::sound_actor& s3gl::actor_manager::get_actor(std::size_t hash)
{
    return *sm_sound_actors[hash];
}

std::size_t s3gl::actor_manager::get_actor_hash(std::string_view name)
{
    for(auto& s : sm_sound_key_map)
    {
        std::cout << s.first << ':' << s.second << '\n';
    }
    std::cout << name << '\n';
    if(sm_sound_key_map.contains(std::string(name)))
    {
        return sm_sound_key_map.at(std::string(name));
    }
    else std::cout << "[WARNING]: no actor found for given name";
    return 0;
}

void s3gl::actor_manager::listen_pos(const glm::vec3& pos, const std::array<float, 6>& orientation)
{
    alListenerfv(AL_ORIENTATION, orientation.data());
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}
