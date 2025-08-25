#include "s3gl.h"

float s3gl::last_time = 0.0f;
// 64-bit FNV-1a parameters (standard, stable)
static constexpr std::uint64_t FNV64_OFFSET = 14695981039346656037ull;
static constexpr std::uint64_t FNV64_PRIME  = 1099511628211ull;

s3gl::timer::timer()
    : m_start(std::chrono::high_resolution_clock::now()) { }

void s3gl::timer::reset()
{
    m_start = std::chrono::high_resolution_clock::now();
}

float s3gl::timer::since_started()
{
    return std::chrono::duration(std::chrono::high_resolution_clock::now() - m_start).count();
}

s3gl::timer s3gl::timer::operator=(const timer& other)
{
    // yes copying is intended here
    m_start = other.m_start;
    return *this;
}

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


std::uint64_t s3gl::fnv1a_hash(const void* data, std::size_t len)
{
    const unsigned char* p = static_cast<const unsigned char*>(data);
    std::uint64_t h = FNV64_OFFSET;

#if defined(__SIZEOF_INT128__)
    // Use 128-bit intermediate multiply to give the compiler freedom to optimize
    // (result is still reduced mod 2^64 by the cast, matching FNV-1a spec)
    const unsigned __int128 P = static_cast<unsigned __int128>(FNV64_PRIME);
    for (std::size_t i = 0; i < len; ++i) 
    {
        h ^= static_cast<std::uint64_t>(p[i]);
        h = static_cast<std::uint64_t>(static_cast<unsigned __int128>(h) * P);
    }
#else
    // Pure 64-bit fallback
    for (std::size_t i = 0; i < len; ++i) 
    {
        h ^= static_cast<std::uint64_t>(p[i]);
        h *= FNV64_PRIME;
    }
#endif
    return h;
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





