#pragma once
#include "mesh.h"

namespace s3gl
{
    class asset_manager
    {
    private:
        static inline std::unordered_map<std::size_t, std::shared_ptr<mesh>> sm_mesh;
        static inline std::hash<std::string> sm_hasher;
        static inline s3gl::asset_manager sm_instance;
    public:
        asset_manager() = default;
        asset_manager(const asset_manager&) = delete;
        asset_manager& operator=(const asset_manager&) = delete;

        static asset_manager& get();
        static std::size_t new_mesh(const std::string& objname, const std::string& objfpath, const std::string& texfpath, const std::string& fragfpath, const std::string& vertfpath, int tex_unit, const glm::vec3& pos);
        static const s3gl::mesh& get_mesh(std::size_t hash);
        static void rm_mesh(std::size_t hash);
    };
}