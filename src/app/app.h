#include "../scene/scene.h"
#include "../render/imgui_element.h"

class app
{
private:
    GLFWwindow* window;

public:
    app();

    void loop();

    ~app();
};