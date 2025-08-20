#include "app.h"

app::app()
{
    // init glfw but not glew and gl
    if(s3gl::init((std::uint16_t)s3gl::engine::INIT_GLFW) != 0)
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
    if(s3gl::init((std::uint16_t)s3gl::engine::INIT_GLEW) != 0)
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
    // shaders
    s3gl::actor_manager::init();
    
    s3gl::asset_manager::shad_hash shad_hash = s3gl::asset_manager::new_shad("normal", "src/shaders/vert.glsl", "src/shaders/frag.glsl");
    s3gl::asset_manager::tex_hash tex_hash_flower = s3gl::asset_manager::new_tex("normal", "assets/tex/2.jpg", shad_hash);
    s3gl::asset_manager::tex_hash tex_hash_grass = s3gl::asset_manager::new_tex("normal1", "assets/tex/grass.png", shad_hash);
    // meshes
    // std::size_t cube_hash  = s3gl::asset_manager::new_mesh("cube", "assets/obj/torus.obj", shad_hash, tex_hash_flower, glm::vec3(3.0f, 3.0f, -3.0f));
    // std::size_t cone_hash  = s3gl::asset_manager::new_mesh("cone", "assets/obj/cone.obj", shad_hash, tex_hash_flower, glm::vec3(-3.0f, -2.0f, 3.0f));
    // std::size_t cone2_hash = s3gl::asset_manager::new_mesh("cone2", "assets/obj/cone.obj", shad_hash, tex_hash_flower, glm::vec3(-3.0f, -2.0f, -3.0f));
    // std::size_t cube2_hash = s3gl::asset_manager::new_mesh("cube2", "assets/obj/cube.obj", shad_hash, tex_hash_flower, glm::vec3(-3.0f, -2.0f, -3.0f));
    // std::size_t cube3_hash = s3gl::asset_manager::new_mesh("cube3", "assets/obj/cube.obj", shad_hash, tex_hash_flower, glm::vec3(0.0f, 2.0f, 0.0f));
    // std::size_t land_hash  = s3gl::asset_manager::new_mesh("land", "assets/obj/terrain.obj", shad_hash, tex_hash_grass, glm::vec3(0.0f, -10.0f, 0.0f));
    
    // new class (still heavy WIP)
    s3gl::scene scene1("assets/scenes/scene.json");
    
    // objects that actually do stuff
    s3gl::mesh& cube2 = scene1.get_mesh_ref("cube2");
    s3gl::mesh& land  = scene1.get_mesh_ref("land");
    s3gl::mesh& cube3  = scene1.get_mesh_ref("cube3");
    
    
    glm::vec4 light_col = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 light_pos = glm::vec3(2.0f, 2.0f, 2.0f);
    
    float b[3] = {135.0f / 255.0f, 234.0f / 255.0f, 255.0f / 255.0f};
    float b2[3] = {1.0f, 1.0f, 1.0f};
    float a[3] = {2.0f, 2.0f, 2.0f};
    float rot_speed = 0.0f;

    s3gl::camera cam(glm::vec3(0.0f, 0.0f, 2.0f));
    int light_preset = (std::uint16_t)s3gl::engine::LIGHTING_DIRECT;
    cam.fov = 90.0f;
    
    bool box_grounded = true;
    bool box_following = false;
    s3gl::actor_manager::listen_pos(cam.pos, cam.get_orientation());
    s3gl::actor_manager::play_actor(1LL);
    
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(b[0], b[1], b[2], 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        light_col = glm::vec4(b2[0], b2[1], b2[2], 1.0f);
        light_pos = cam.pos;
        cube2.pos.x += (cam.pos.x - 2.0f - cube2.pos.x) / 50.0f;
        cube2.pos.z += (cam.pos.z - 2.0f - cube2.pos.z) / 50.0f;
        if(box_following)
        {
            if(box_grounded)
                cube2.pos.y = land.get_height_data(cube2.pos) - 8.0f;
            else
                cube2.pos.y += (cam.pos.y - 2.0f - cube2.pos.y) / 50.0f;
        }

        
        cam.speed = rot_speed;

        cube3.pos.z -= 0.03f;
        
        // if (!ImGui::GetIO().WantCaptureMouse)     
            cam.inputs(window, land.get_height_data(cam.pos) - 5.0f);
        cam.update_matrix(0.1f, 10000.0f);
        
        s3gl::actor_manager::listen_pos(cam.pos, cam.get_orientation());
        // all s3gl rendering
        s3gl::renderer::render(cam, glm::vec3(light_pos), glm::vec4(light_col), light_preset);
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
        ImGui::SliderFloat("Speed", &rot_speed, 0.0f, 0.5f);
        ImGui::SliderFloat("Gravity", &cam.gravity, 1.0f, 5.0f);
        ImGui::SliderFloat("Sens", &cam.sens, 0.0f, 5.0f);
        // ImGui::SliderFloat3("Light Pos", a, -10.0f, 10.0f);
        ImGui::Checkbox("Grounded", &cam.grounded);
        ImGui::Checkbox("Box Grounded", &box_grounded);
        ImGui::End();
        // lighting
        ImGui::Begin("Lighting");
        if(ImGui::Button("Low Graphics"))
            light_preset = (std::uint16_t)s3gl::engine::LIGHTING_NOLIGHT;
        else if(ImGui::Button("Direct Light"))
            light_preset = (std::uint16_t)s3gl::engine::LIGHTING_DIRECT;
        else if(ImGui::Button("Point Light"))
            light_preset = (std::uint16_t)s3gl::engine::LIGHTING_POINT;
        else if(ImGui::Button("Spot Light"))
            light_preset = (std::uint16_t)s3gl::engine::LIGHTING_SPOT;
        else if(ImGui::Button("Sun Light"))
            light_preset = (std::uint16_t)s3gl::engine::LIGHTING_SUN;
        ImGui::End();
        //fps
        int fps = s3gl::calc_fps();
        ImGui::Begin("FPS");
        ImGui::Text("%d", fps);
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