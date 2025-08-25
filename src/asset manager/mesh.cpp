#include "mesh.h"

s3gl::mesh::mesh()
{
    m_init = false;
}

s3gl::mesh::mesh(const std::string& objfpath, const s3gl::shader& shad, s3gl::texture& tex, glm::vec3 pos)
    :   m_shad(shad), m_tex(tex), pos(pos), add_to_render_list(false)
{
    m_init = false;
    
    std::ifstream obj_file(objfpath);
    if(obj_file)
    {
        m_shad.build();
        // binding vertex array object
        m_vao.bind();
        m_shad.attach();
        // setting up containers for .obj vertices extraction
        std::vector<glm::vec3> verts;
        std::vector<glm::vec3> norms;
        std::vector<glm::vec2> texcoords;
        std::vector<vertex> verticies;
        std::string line;
        while (std::getline(obj_file, line)) 
        {
            if (line.empty() || line[0] == '#') 
                continue;
                
            std::istringstream ss(line);
            std::string type;
            ss >> type;

            if (type == "v") 
            {
                float x, y, z;
                ss >> x >> y >> z;
                verts.emplace_back(x, y, z);
            } else if (type == "vt") 
            {
                float u, v;
                ss >> u >> v;
                texcoords.emplace_back(u, v);
            } else if (type == "vn") 
            {
                float x, y, z;
                ss >> x >> y >> z;
                norms.emplace_back(x, y, z);
            } else if (type == "f") 
            {
                std::string v1, v2, v3;
                ss >> v1 >> v2 >> v3;
                std::array<std::string, 3> faceVerts = {v1, v2, v3};

                for (const auto& fv : faceVerts) 
                {
                    int vi = 0, ti = 0, ni = 0;
                    sscanf(fv.c_str(), "%d/%d/%d", &vi, &ti, &ni);
                    vertex v;
                    v.v = verts[vi - 1];
                    v.tex = texcoords[ti - 1];
                    v.norm = norms[ni - 1];
                    verticies.push_back(v);
                }
            }
        }

        // i know i know, forced order isn't great,
        // but genuinly what else would you do?
        // I can't be bothered getting a 3rd
        // party library to load meshes so this is
        // the best I can do
        std::vector<float> vert_data;
        for(int i = 0; i < verticies.size(); i++)
        {
            vert_data.push_back(verticies[i].v.x);
            vert_data.push_back(verticies[i].v.y);
            vert_data.push_back(verticies[i].v.z);
        
            m_verts.push_back(glm::vec3(verticies[i].v));

            vert_data.push_back(verticies[i].norm.x);
            vert_data.push_back(verticies[i].norm.y);
            vert_data.push_back(verticies[i].norm.z);

            vert_data.push_back(verticies[i].tex.x);
            vert_data.push_back(verticies[i].tex.y);
        }
        // can use vert_data.data() because memory
        // won't be freed by vector until end of
        // mesh constructor ->
        m_vbo = VBO(vert_data.data(), vert_data.size() * sizeof(float));
        
        m_num_verts = verticies.size();
        m_init = true;
        // -> here
    }
}

void s3gl::mesh::link_atribute(const std::vector<GLuint>& layout_pos, const std::vector<GLuint>& num_comps, GLsizeiptr stride, const std::vector<void*>& offsets)
{   
    // want to use a static_assert here to clean up code
    if((layout_pos.size() == num_comps.size() && num_comps.size() == offsets.size()) &&
        m_init)
    {
        for(int i = 0; i < layout_pos.size(); i++)
        {
            m_vao.link_attrib(m_vbo, layout_pos[i], num_comps[i], GL_FLOAT, stride, offsets[i]);
        }
        m_vao.unbind();
        m_vbo.unbind();
    }
    // all functions throw exceptions to avoid future debugging headaches 
    else throw "Attempted use of an uninitialised mesh object (mesh::link_atrib)\n";
}

void s3gl::mesh::set_tex_flags(GLuint pname, GLuint params)
{
    if(m_init)
    {
        m_tex.set_param(pname, params);
    }
    else throw "Attempted use of an uninitialised mesh object (mesh::tex_flags single)\n";
}

void s3gl::mesh::set_tex_flags(int preset)
{
    if(m_init)
    {
        switch (preset)
        {
        case s3gl::MESH_TEX_PRESET_1:
            m_tex.set_param(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_tex.set_param(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            m_tex.set_param(GL_TEXTURE_WRAP_S, GL_REPEAT);
            m_tex.set_param(GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        default:
            std::cout << "[WARNING]: Preset not recognised\n";
            m_tex.set_param(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_tex.set_param(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            m_tex.set_param(GL_TEXTURE_WRAP_S, GL_REPEAT);
            m_tex.set_param(GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        }
    }
    else throw "Attempted use of an uninitialised mesh object (mesh::tex_flags preset)\n";
}

float s3gl::mesh::get_height_data(const glm::vec3& pos)
{
    if(m_init)
    {    
        float min_dist_squared = std::numeric_limits<float>::max();
        float y_close = 0.0f;

        for (const auto& vert : m_verts) 
        {
            float dx = vert.x - pos.x;
            float dy = vert.y - pos.y;
            float dz = vert.z - pos.z;
            float dist_squared = dx * dx + dy * dy + dz * dz;

            if (dist_squared < min_dist_squared) 
            {
                min_dist_squared = dist_squared;
                y_close = vert.y - pos.y;
            }
        }
        return y_close;
    }
    else throw "Attempted use of an uninitialised mesh object (mesh::height_data)\n";
}

void s3gl::mesh::draw(camera& cam, const glm::vec3& light_pos, const glm::vec4& light_col, int light_preset)
{
    if(m_init && add_to_render_list)
    {
        m_model = glm::mat4(1.0f);
        m_model = glm::translate(m_model, pos);

        glm::vec3 light_pos_orient = light_pos;

        m_shad.activate();
        GLuint loc = glGetUniformLocation(m_shad.id, "cam_pos");
        if(loc == -1)
            std::cout << "Couldn't find cam_pos loc\n";
        glUniform3f(loc, cam.pos.x, cam.pos.y, cam.pos.z);

        GLuint loc2 = glGetUniformLocation(m_shad.id, "light_col");
        if(loc2 == -1)
            std::cout << "Couldn't find light_col loc\n";
        glUniform4f(loc2, light_col.x, light_col.y, light_col.z, light_col.w);

        GLuint loc3 = glGetUniformLocation(m_shad.id, "light_pos");
        if(loc3 == -1)
            std::cout << "Couldn't find light_pos loc\n";
        glUniform3f(loc3, light_pos_orient.x, light_pos_orient.y, light_pos_orient.z);

        GLuint loc4 = glGetUniformLocation(m_shad.id, "m_model");
        if(loc4 == -1)
            std::cout << "Couldn't find m_model loc\n";
        glUniformMatrix4fv(loc4, 1, GL_FALSE, glm::value_ptr(m_model));

        GLuint loc5 = glGetUniformLocation(m_shad.id, "light_preset");
        if(loc4 == -1)
            std::cout << "Couldn't find m_model loc\n";
        glUniform1i(loc5, light_preset - 0x20);


        cam.matrix(m_shad, "camMat");
        m_tex.bind();
        m_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, m_num_verts);
    }
    else if(!add_to_render_list) std::cout << "[WARNING]: Attempted drawing of a mesh not on render list\n";
    else throw "Attempted use of an uninitialised mesh object (mesh::draw)\n";
}