#include "../actor/actor_manager.h"

namespace s3gl
{
    class scene
    {
    private:
        // I know it's bad but I can't think of another way
        std::unordered_map<size_t, std::vector<std::pair<std::string, glm::vec3>>> m_stage_mesh_list;
        std::unordered_map<size_t, std::vector<std::pair<std::string, glm::vec3>>> m_stage_sound_list;
        std::string m_jsonFpath;
        bool m_active;
        timer m_time;
        size_t m_stage;
        size_t m_num_stages;
    public:
        scene(std::string_view fpath_json);
        mesh& get_mesh_ref(std::string_view name);

        bool operator()();

        void load();
        void start();
        void set_stage(size_t stage_num);
        void update();
        void end();
    };
}