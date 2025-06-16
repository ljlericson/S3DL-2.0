#include "app.h"

app::app()
{
    // init glfw but not glew and gl
    if(s3gl::init(s3gl::INIT_GLFW) != 0)
    {
        throw s3gl::exception("Failed to init glfw\n");
    }
    
    // create the window
    window = glfwCreateWindow(WIDTH, HEIGHT, "S3GL", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "[ERROR]: failed to create window\n";
        throw s3gl::exception("Failed to create window in app construction process\n");
    }
    // make the window the context
    glfwMakeContextCurrent(window);
    
    // now init glew after we inited glfw and made window context
    if(s3gl::init(s3gl::INIT_GLEW) != 0)
    {
        throw s3gl::exception("Failed to init glew\n");
    }
    // set view port and change background colour
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSwapBuffers(window);

    // init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // white burns my eyes
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);      // Always use this with culling
    glEnable(GL_CULL_FACE);       // Enable face culling
    glCullFace(GL_BACK);          // Don't draw back faces
    glFrontFace(GL_CCW);          // Default winding is counter-clockwise
    glfwSwapInterval(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    stbi_set_flip_vertically_on_load(true);
}

app::~app()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    s3gl::terminate();
}

void app::loop()
{   
    // Hello GitHub!
    ImGuiIO& io = ImGui::GetIO();

    s3gl::mesh cube("assets/obj/torus.obj", "assets/tex/grass.png", "src/shaders/frag.glsl", "src/shaders/vert.glsl", 0, glm::vec3(3.0f, 3.0f, -3.0f));
    cube.link_atribute({0, 1, 2}, {3, 2, 3}, 8 * sizeof(float), {(void*)0, (void*)(3 * sizeof(float)), (void*)(6 * sizeof(float))});
    cube.set_tex_flags(s3gl::MESH_TEX_PRESET_1);
    s3gl::mesh cone("assets/obj/cone.obj", "assets/tex/2.jpg", "src/shaders/frag.glsl", "src/shaders/vert.glsl", 1, glm::vec3(-3.0f, -2.0f, 3.0f));
    cone.link_atribute({0, 1, 2}, {3, 2, 3}, 8 * sizeof(float), {(void*)0, (void*)(3 * sizeof(float)), (void*)(6 * sizeof(float))});
    cone.set_tex_flags(s3gl::MESH_TEX_PRESET_1);

    s3gl::mesh cone2("assets/obj/cone.obj", "assets/tex/2.jpg", "src/shaders/frag.glsl", "src/shaders/vert.glsl", 2, glm::vec3(-3.0f, -2.0f, -3.0f));
    cone2.link_atribute({0, 1, 2}, {3, 2, 3}, 8 * sizeof(float), {(void*)0, (void*)(3 * sizeof(float)), (void*)(6 * sizeof(float))});
    cone2.set_tex_flags(s3gl::MESH_TEX_PRESET_1);

    s3gl::mesh cube2("assets/obj/cube.obj", "assets/tex/2.jpg", "src/shaders/frag.glsl", "src/shaders/vert.glsl", 3, glm::vec3(-3.0f, -2.0f, -3.0f));
    cube2.link_atribute({0, 1, 2}, {3, 2, 3}, 8 * sizeof(float), {(void*)0, (void*)(3 * sizeof(float)), (void*)(6 * sizeof(float))});
    cube2.set_tex_flags(s3gl::MESH_TEX_PRESET_1);

    s3gl::mesh land("assets/obj/bigland.obj", "assets/tex/grass.png", "src/shaders/frag.glsl", "src/shaders/vert.glsl", 4, glm::vec3(0.0f, -10.0f, 0.0f));
    land.link_atribute({0, 1, 2}, {3, 2, 3}, 8 * sizeof(float), {(void*)0, (void*)(3 * sizeof(float)), (void*)(6 * sizeof(float))});
    land.set_tex_flags(s3gl::MESH_TEX_PRESET_1);

    glm::vec4 light_col = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 light_pos = glm::vec3(2.0f, 2.0f, 2.0f);

    float b[3] = {135.0f / 255.0f, 234.0f / 255.0f, 255.0f / 255.0f};
    float b2[3] = {1.0f, 1.0f, 1.0f};
    float a[3] = {2.0f, 2.0f, 2.0f};
    float rot_speed = 0.0f;

    s3gl::camera cam(glm::vec3(0.0f, 0.0f, 2.0f));
    int light_preset = s3gl::LIGHTING_DIRECT;
    cam.fov = 90.0f;

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(b[0], b[1], b[2], 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        light_col = glm::vec4(b2[0], b2[1], b2[2], 1.0f);
        light_pos = glm::vec3(a[0], a[1], a[2]);
        cube2.pos = glm::vec3(a[0], a[1], a[2]);
        
        cam.speed = rot_speed;
        
        if (!ImGui::GetIO().WantCaptureMouse)     
            cam.inputs(window, land.get_height_data(cam.pos) - 5.0f);
        cam.update_matrix(0.1f, 10000.0f);

        cube.draw(cam, glm::vec3(light_pos), glm::vec4(light_col), light_preset);
        cone.draw(cam, glm::vec3(light_pos), glm::vec4(light_col), light_preset);
        cone2.draw(cam, glm::vec3(light_pos), glm::vec4(light_col), light_preset);
        cube2.draw(cam, glm::vec3(light_pos), glm::vec4(light_col), light_preset);
        land.draw(cam, glm::vec3(light_pos), glm::vec4(light_col), light_preset);

        // ALL imgui rendering
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // colour picker
        ImGui::Begin("Background Colour");
        if(ImGui::Button("Print Something"))
            std::cout << "something\n";
        ImGui::ColorPicker3("Background Colour", b);
        ImGui::ColorPicker3("Light Colour", b2);
        ImGui::End();
        // tri toggle
        ImGui::Begin("Movement");
        ImGui::SliderFloat("Speed", &rot_speed, 0.0f, 0.1f);
        ImGui::SliderFloat("Gravity", &cam.gravity, 1.0f, 5.0f);
        ImGui::SliderFloat("Sens", &cam.sens, 0.0f, 5.0f);
        ImGui::SliderFloat3("Light Pos", a, -10.0f, 10.0f);
        ImGui::Checkbox("Grounded", &cam.grounded);
        ImGui::End();
        // lighting
        ImGui::Begin("Lighting");
        if(ImGui::Button("Low Graphics"))
            light_preset = s3gl::LIGHTING_NOLIGHT;
        else if(ImGui::Button("Direct Light"))
            light_preset = s3gl::LIGHTING_DIRECT;
        else if(ImGui::Button("Point Light"))
            light_preset = s3gl::LIGHTING_POINT;
        ImGui::End();
        //fps
        ImGui::Begin("FPS");
        ImGui::Text("%d", s3gl::calc_fps());
        ImGui::End();
        // rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffers 
        glfwSwapBuffers(window);
        // poll events
        glfwPollEvents();
    }
}