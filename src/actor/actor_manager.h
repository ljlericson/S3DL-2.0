#include "actor.h"

namespace s3gl
{
    class actor_manager
    {
    private:
        static inline std::unordered_map<std::size_t, std::unique_ptr<sound_actor>> sm_sound_actors;
        static inline std::unordered_map<std::string, std::size_t> sm_sound_key_map;
        static inline ALCdevice* m_device;
        static inline ALCcontext* m_context;
        static inline std::hash<std::string> sm_hasher;

    public:
        actor_manager() = default;
        actor_manager(const actor_manager&) = delete;
        actor_manager& operator=(const actor_manager&) = delete;

        static void init();
        static std::size_t new_sound_actor(s3gl::asset_manager::audio_buffer_hash abhash, 
                                        s3gl::asset_manager::mesh_hash mhash,
                                        std::string_view name,
                                        const glm::vec3& pos);
        static void queue_sound_events();
        static void stop_sounds();
        static void clear();
        static s3gl::sound_actor& get_actor(std::size_t hash);
        static std::size_t get_actor_hash(std::string_view name);
        static void listen_pos(const glm::vec3& pos, const std::array<float, 6>& orientation);
    };
}