#include "mesh.h"

s3gl::mesh::mesh()
{
    has_init = false;
}

s3gl::mesh::mesh(const std::string& objfpath, const std::string& texfpath, const std::string& fragfpath, const std::string& vertfpath, int tex_unit, glm::vec3 pos)
{
    has_init = false;
    
    std::ifstream obj_file(objfpath);
    if(obj_file)
    {
        // setting up mesh aata
        this->pos = pos;
        objTex = texture(texfpath.c_str(), GL_TEXTURE0 + tex_unit, GL_TEXTURE_2D);
        shad = shader(vertfpath, fragfpath);
        // binding vertex array object
        vao.bind();
        shad.attach();
        objTex.bind(shad.id);
        // setting up containers for .obj vertices extraction
        std::vector<glm::vec3> verts;
        std::vector<glm::vec3> norms;
        std::vector<glm::vec2> texcoords;
        std::vector<vertex> verticies;
        std::cout << "[Info]: Loading mesh\n";
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
        
        for(int i = 0; i < verticies.size(); i++)
        {
            vert_data.push_back(verticies[i].v.x);
            vert_data.push_back(verticies[i].v.y);
            vert_data.push_back(verticies[i].v.z);

            vert_data.push_back(verticies[i].norm.x);
            vert_data.push_back(verticies[i].norm.y);
            vert_data.push_back(verticies[i].norm.z);

            vert_data.push_back(verticies[i].tex.x);
            vert_data.push_back(verticies[i].tex.y);
        }
        vbo = VBO(vert_data.data(), vert_data.size() * sizeof(float));
        
        num_verts = verticies.size();
        has_init = true;
    }
}

void s3gl::mesh::link_atribute(std::vector<GLuint> layout_pos, std::vector<GLuint> num_comps, GLsizeiptr stride, std::vector<void*> offsets)
{
    if((layout_pos.size() == num_comps.size() && num_comps.size() == offsets.size()) &&
        has_init)
    {
        for(int i = 0; i < layout_pos.size(); i++)
        {
            vao.link_attrib(vbo, layout_pos[i], num_comps[i], GL_FLOAT, stride, offsets[i]);
        }
        vao.unbind();
        vbo.unbind();
    }
}

void s3gl::mesh::set_tex_flags(GLuint pname, GLuint params)
{
    if(has_init)
    {
        objTex.set_param(pname, params);
    }
}

void s3gl::mesh::set_tex_flags(int preset)
{
    switch (preset)
    {
    case s3gl::MESH_TEX_PRESET_1:
        objTex.set_param(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        objTex.set_param(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        objTex.set_param(GL_TEXTURE_WRAP_S, GL_REPEAT);
        objTex.set_param(GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    default:
        std::cout << "[ERROR]: Preset not recognised\n";
        objTex.set_param(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        objTex.set_param(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        objTex.set_param(GL_TEXTURE_WRAP_S, GL_REPEAT);
        objTex.set_param(GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    }
}

void s3gl::mesh::draw(camera& cam, const glm::vec3& light_pos, const glm::vec4& light_col)
{
    if(has_init)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pos);

        shad.activate();
        GLuint loc = glGetUniformLocation(shad.id, "cam_pos");
        if(loc == -1)
            std::cout << "Couldn't find cam_pos loc\n";
        glUniform3f(loc, cam.pos.x, cam.pos.y, cam.pos.z);

        GLuint loc2 = glGetUniformLocation(shad.id, "light_col");
        if(loc2 == -1)
            std::cout << "Couldn't find light_col loc\n";
        glUniform4f(loc2, light_col.x, light_col.y, light_col.z, light_col.w);

        GLuint loc3 = glGetUniformLocation(shad.id, "light_pos");
        if(loc3 == -1)
            std::cout << "Couldn't find light_pos loc\n";
        glUniform3f(loc3, light_pos.x, light_pos.y, light_pos.z);

        GLuint loc4 = glGetUniformLocation(shad.id, "model");
        if(loc4 == -1)
            std::cout << "Couldn't find model loc\n";
        glUniformMatrix4fv(loc4, 1, GL_FALSE, glm::value_ptr(model));

        cam.matrix(shad, "camMat");
        objTex.bind();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, num_verts);
    }
}