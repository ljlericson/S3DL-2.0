#include "../actor/actor_manager.h"

namespace s3gl
{
    class scene
    {
    private:
        std::vector<std::unique_ptr<s3gl::basic_mesh>> m_bmeshes;
    public:
        scene(std::string_view fpath_json);
    };
}