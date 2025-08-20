#include "../actor/actor_manager.h"

namespace s3gl
{
    class scene
    {
    private:
        std::unordered_map<std::string, std::size_t> m_mesh_hash_map;
    public:
        scene(std::string_view fpath_json);

        mesh& get_mesh_ref(std::string_view name);
    };
}