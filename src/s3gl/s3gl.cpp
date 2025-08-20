#include "s3gl.h"

float s3gl::last_time = 0.0f;

int s3gl::init(int flags)
{
    switch(flags)
    {
    case INIT_GLFW:
        // init glfw
        if(glfwInit() != GLFW_TRUE)
        {
            const char* description;
            int code = glfwGetError(&description);

            std::cerr << "GLFW initialization failed, error code: " << code << "\n";
            if(description) 
                std::cerr << "Error description: " << description << "\n";
            else 
                std::cerr << "No error description available.\n";
            return -1;
        }
        // provide glfw with relevent info
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        break;
    case INIT_GLEW:
        // init glew
        if(glewInit() != 0)
        {
            std::cerr << "GLEW failed to init, error: " << glGetError() << '\n';
            return -1;
        }
        break;
    }
    return 0;
}

std::vector<std::string_view> s3gl::split_string(std::string_view input, char sep) 
{
    std::vector<std::string_view> result;

    size_t start = 0;
    while (start < input.size()) 
    {
        size_t end = input.find(sep, start);
        if (end == std::string_view::npos) 
        {
            result.emplace_back(input.substr(start));
            break;
        }
        result.emplace_back(input.substr(start, end - start));
        start = end + 1;
    }

    return result;
}


int s3gl::calc_fps() 
{ 
    // Frame info
    float delta_time = glfwGetTime() - last_time;
    last_time = glfwGetTime();
    return 1 / delta_time; 
}

void s3gl::terminate()
{
    glfwTerminate();
}

s3gl::exception::exception(const std::string& except)
{
    this->except = except;
}

std::string s3gl::exception::what()
{
    return std::string("|[FATAL ERROR]| e.what(): " + except);
}





