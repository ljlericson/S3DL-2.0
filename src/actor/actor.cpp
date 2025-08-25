#include "actor.h"

s3gl::sound_actor::sound_actor() 
    :   m_init(false), m_play(false) { }

s3gl::sound_actor::sound_actor(s3gl::asset_manager::audio_buffer_hash ab_hash, s3gl::asset_manager::mesh_hash mesh_hash, const glm::vec3& pos)
    :   m_init(true), m_pos(pos), m_ABhash(ab_hash), m_Mhash(mesh_hash)
{
    if(m_Mhash != 0)
        asset_manager::get_mesh(m_Mhash).pos = m_pos;
    alGenSources(1, &m_source);
    alSourcei(m_source, AL_BUFFER, asset_manager::get_WAV_buffer(m_ABhash));
    alSource3f(m_source, AL_POSITION, m_pos.x, m_pos.y, m_pos.z);
    alSourcef(m_source, AL_GAIN, 1.0f);
    alSource3f(m_source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(m_source, AL_SOURCE_RELATIVE, AL_FALSE);
    alSourcef(m_source, AL_ROLLOFF_FACTOR, 1.0f);
    alSourcef(m_source, AL_REFERENCE_DISTANCE, 5.0f);  // Distance at which volume is full
    alSourcef(m_source, AL_MAX_DISTANCE, 100.0f);      // Distance after which volume stays constant
}

s3gl::sound_actor::~sound_actor()
{
    if(m_init)
        alDeleteSources(1, &m_source);
}

void s3gl::sound_actor::play_sound(ALCdevice* device, ALCcontext* context)
{
    if(m_init && m_play)
    {
        if(m_Mhash != 0)
            asset_manager::get_mesh(m_Mhash).pos = m_pos;
        alSource3f(m_source, AL_POSITION, m_pos.x, m_pos.y, m_pos.z);
        if (ALenum err = alGetError(); err != AL_NO_ERROR)
            std::cerr << "OpenAL error1: " << err << "\n";
        alSourcei(m_source, AL_LOOPING, AL_TRUE);
        if (ALenum err = alGetError(); err != AL_NO_ERROR)
            std::cerr << "OpenAL error2: " << err << "\n";
        alSourcePlay(m_source);
        if (ALenum err = alGetError(); err != AL_NO_ERROR)
            std::cerr << "OpenAL error3: " << err << "\n";
    }
    else if(!m_play) std::cout << "[WARNING]: Actor is not on play list\n";
    else throw s3gl::exception("Attempted use of a non-initialised sound actor\n");
}

void s3gl::sound_actor::stop_sound()
{
    if(m_init)
    {
        alSourceStop(m_source);
    }
    else throw s3gl::exception("Attempted use of a non-initialised sound actor\n");
}