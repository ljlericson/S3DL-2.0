#pragma once
#include "mesh.h"

namespace s3gl
{
    class asset_manager
    {
    private:
        static inline std::unordered_map<std::size_t, std::unique_ptr<shader>> sm_shaders;
        static inline std::unordered_map<std::size_t, std::unique_ptr<texture>> sm_textures;
        static inline std::unordered_map<std::size_t, std::unique_ptr<mesh>> sm_meshes;
        static inline std::unordered_map<std::size_t, ALuint> sm_audio_buffers;
        static inline std::hash<std::string> sm_hasher;
        static asset_manager sm_instance;
    public:
        // make code clearer because size_t
        // representing an object could be
        // confusing
        using shad_hash = std::size_t;
        using tex_hash  = std::size_t;
        using mesh_hash = std::size_t;
        using audio_buffer_hash = std::size_t;
        // deleting copy and asign operators to 
        // avoid multiple instances of class or
        // initialising/copying of another obj
        // using singlton isntance
        asset_manager() = default;
        asset_manager(const asset_manager&) = delete;
        asset_manager& operator=(const asset_manager&) = delete;
        // for use as a singleton
        static asset_manager& get();
        // keeping functions static for use of class
        // as namespace with private attributes to
        // avoid weird stuff
        static std::size_t new_tex(std::string_view name_tex, std::string_view fpath, std::size_t shad_hash);
        static std::size_t new_shad(std::string_view name_shad, std::string_view fpathVert, std::string_view fpathFrag);
        static std::size_t new_mesh(std::string_view name_mesh, const std::string& objfpath, std::size_t shad_hash, std::size_t tex_hash, const glm::vec3& pos);
        static std::size_t new_WAV_buffer(std::string_view name_audio, std::string_view fpathWAV);
        // rm == remove
        static s3gl::mesh& get_mesh(std::size_t hash);
        static const ALuint get_WAV_buffer(std::size_t hash);
        static std::size_t get_mesh_hash(std::string_view name);
        static std::size_t get_shad_hash(std::string_view name);
        static std::size_t get_tex_hash(std::string_view name);
        static const std::unordered_map<std::size_t, std::unique_ptr<mesh>>& get_mesh_map();
        static void rm_mesh(std::size_t hash);
        static void clear();
    };
}