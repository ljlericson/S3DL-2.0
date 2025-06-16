#include "texture.h"

namespace s3gl
{
    class camera
    {
    private:
        // jump variables
        bool jump;
        int jump_ticks;
        bool first_jump;
        float yOriginal;
        // mouse input
        bool first_click;
        // opengl matrix math
        glm::vec3 up;
        glm::vec3 orientation;
        glm::mat4 cam_mat;

    public:
        // use mutable values
        glm::vec3 pos;
        bool grounded;
        float speed;
        float sens;
        float fov;
        float gravity;
        
        
        camera(glm::vec3 pos);

        void update_matrix(float fnear, float ffar);

        void matrix(const shader& shader, const char* uniform);

        void inputs(GLFWwindow* window, float height_data);
    };
}