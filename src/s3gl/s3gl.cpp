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

std::vector<std::string> s3gl::split_string(const std::string& i, char separating_char)
{
    std::vector<std::string> out;
    // index_f and index_b to keep track of individual word pos
    int index_f, index_b = 0;

    // iterates through the characters in string 'i'
    for(int  j = 0; j < i.length(); j++)
    {
        // updating front index
        index_f = j;
        // checking if i[j] is the separating char or is at the end
        if(i[j] == separating_char || j == i.length() - 1)
        {
            std::string to_go;
            // resizing string due to to_go being freed every 
            // iteration
            to_go.resize(index_f - index_b);

            // copying chars over to new string based on foward and
            // back indexes
            for(int t = 0; t < (index_f - index_b); t++)    
                to_go[t] = i[t + index_b];

            // adding the last character on in the case it is the 
            // last word in the string
            if(j == i.size() - 1)
                to_go.push_back(i[j]);

            // adding newly created string to the vector and updating
            // the back index
            out.push_back(to_go);
            index_b = j + 1;
        }
    }
    return out;
}

int s3gl::calc_fps() 
{ 

    // Frame info
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - last_time;
    last_time = currentTime;
    return 1 / deltaTime; 
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
    return except;
}





