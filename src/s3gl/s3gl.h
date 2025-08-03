/*
    COPYRIGHT (C) Linus Ericson 2024
    Email: ljlericson@protonmail.com

    This header file and other files in the src of this 
    project is licensed by the creative commons attribution
    non-commercial-noderivatrives 4.0 international license

    License: CC BY-NC-ND 4.0
    License link: https://creativecommons.org/licenses/by-nc-nd/4.0/deed.en

    This is a remake of another project I had called 's3dl'
    or 'simple 3d library', it was badly programmed and 
    had lots of bugs. Hopefully s3gl or s3dl 2.0 turns out 
    slightly better, I hope to not use SDL and instead 
    experiment with other input libaries.
*/
#pragma once
// std libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <string_view>
#include <memory>
#include <vector>
#include <map>
#include <array>
#include <limits>
#include <cmath>
// opengl
#include <GL/glew.h>
#include <glfw/glfw3.h>
// gl maths library
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
// the holy grail?
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// stb
#include <stb/stb_image.h>
// doctests
// #include <doctest/extensions/doctest_mpi.h>
// scr macros
#define WIDTH 1280
#define HEIGHT 720

namespace s3gl
{
    extern float last_time;

    typedef unsigned int uint;

    typedef enum
    {
        INIT_GLFW = 0x00,
        INIT_GLEW = 0x01,

        MESH_TEX_PRESET_1 = 0x11,

        LIGHTING_NOLIGHT = 0x20,
        LIGHTING_DIRECT = 0x21,
        LIGHTING_POINT = 0x22,
    } engine;

    enum class imgui_type
    {
        
    };


    int init(int flags);
    
    // Parses individual words in a string that are separated by a chosen character
    // \param std::string i string input
    // \param char separating_char character used to separate string put ' ' if unsure
    // \return std::vector<std::string> string vector where elemets are individual
    // words that are sorted in order first appeared
    std::vector<std::string> split_string(const std::string& i, char separating_char);

    int calc_fps();

    void terminate();

    struct vertex
    {
        glm::vec3 v;
        glm::vec2 tex;
        glm::vec3 norm;
    };

    class exception
    {
    private:
        std::string except;
    public:
        exception(const std::string& except);
        std::string what();
    }; 
};

