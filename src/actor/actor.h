#include "../asset manager/manager.h"

namespace s3gl
{
    
    class sound_actor
    {
    private:
        bool m_init;
        // OpenAL Source ID
        ALuint m_source;
        // accosiated mesh and
        // audio buffer
        asset_manager::mesh_hash m_Mhash;
        asset_manager::mesh_hash m_ABhash;
    public:
        glm::vec3 m_pos;
        bool m_play;
        sound_actor();
        ~sound_actor();
        sound_actor(s3gl::asset_manager::audio_buffer_hash ab_hash, 
                    s3gl::asset_manager::mesh_hash mesh_hash, 
                    const glm::vec3& pos);

        void play_sound(ALCdevice* device, ALCcontext* context);
        void stop_sound();
    };

    class light_actor
    {

    };
}

