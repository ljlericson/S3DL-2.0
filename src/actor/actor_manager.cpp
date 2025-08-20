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

std::size_t s3gl::actor_manager::new_sound_actor(s3gl::asset_manager::audio_buffer_hash hash, s3gl::asset_manager::mesh_hash mhash, const glm::vec3& pos)
{
    std::size_t ahash = sm_hasher(std::to_string(hash));
    
    sm_sound_actors[ahash] = std::make_unique<sound_actor>(hash, mhash, pos);
    return hash;
}

void s3gl::actor_manager::play_actor(std::size_t hash)
{
    // sm_sound_actors[hash]->play_sound(m_device, m_context);
    for(const auto& actor : sm_sound_actors)
    {
        actor.second->play_sound(m_device, m_context);
    }
}

s3gl::sound_actor& s3gl::actor_manager::get_actor(std::size_t hash)
{
    return *sm_sound_actors[hash];
}

void s3gl::actor_manager::listen_pos(const glm::vec3& pos, const std::array<float, 6>& orientation)
{
    alListenerfv(AL_ORIENTATION, orientation.data());
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

