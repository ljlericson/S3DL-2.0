#include "manager.h"

s3gl::asset_manager s3gl::asset_manager::sm_instance;

s3gl::asset_manager& s3gl::asset_manager::get()
{
    return sm_instance;
}

std::size_t s3gl::asset_manager::new_shad(std::string_view name_shad, std::string_view fpathVert, std::string_view fpathFrag)
{
    std::string s_hash = "shad_";
    s_hash += name_shad;
    std::size_t hash = sm_hasher(s_hash);
    sm_shaders[hash] = std::make_unique<s3gl::shader>(fpathVert, fpathFrag);
    return hash;
}

std::size_t s3gl::asset_manager::new_tex(std::string_view name_tex, std::string_view fpath)
{
    if(sm_textures.size() == 9) 
    {   std::cout << "[ERROR]: Texture slots are full (asset_mannager::new_tex())\n";   return -1;    }
    else
    {
        std::string s_hash = "tex_";
        s_hash += name_tex;
        GLuint tex_slot = GL_TEXTURE0 + sm_textures.size();
        std::size_t hash = sm_hasher(s_hash);
        sm_textures[hash] = std::make_unique<texture>(fpath.data(), tex_slot, GL_TEXTURE_2D);
        return hash;
    }
}

std::size_t s3gl::asset_manager::new_mesh(std::string_view name_mesh, const std::string& objfpath, std::size_t shad_hash, std::size_t tex_hash, const glm::vec3& pos)
{
    // first check that the shader is valid
    if(sm_shaders.find(shad_hash) != sm_shaders.end())
    {
        // create hash for accsesing obj later
        std::string s_hash = "mesh_";
        s_hash += name_mesh;
        std::size_t hash = sm_hasher(s_hash);
        sm_meshes[hash] = std::make_unique<s3gl::mesh>(objfpath, *sm_shaders.at(shad_hash), *sm_textures.at(tex_hash), pos);
        sm_meshes.at(hash)->link_atribute({0, 1, 2}, {3, 2, 3}, 8 * sizeof(float), {(void*)0, (void*)(3 * sizeof(float)), (void*)(6 * sizeof(float))});
        sm_meshes.at(hash)->set_tex_flags(s3gl::MESH_TEX_PRESET_1);
        return hash;
    }
    else
    {   throw s3gl::exception("[ERROR]: Shader hash entered is invalid (new_mesh)\n");  }
}


s3gl::mesh& s3gl::asset_manager::get_mesh(std::size_t hash)
{
    if(sm_meshes.find(hash) != sm_meshes.end())
        return *sm_meshes.at(hash);
    else
        throw s3gl::exception("[ERROR]: Hashed object does not exist (get_mesh)\n");
}

std::size_t s3gl::asset_manager::get_mesh_hash(std::string_view name)
{
    std::string s_hash = "mesh_";
    s_hash += name;
    return sm_hasher(s_hash);
}

std::size_t s3gl::asset_manager::get_shad_hash(std::string_view name)
{
    std::string s_hash = "shad_";
    s_hash += name;
    return sm_hasher(s_hash);
}

const std::unordered_map<std::size_t, std::unique_ptr<s3gl::mesh>>& s3gl::asset_manager::get_mesh_map()
{
    return sm_meshes;
}

void s3gl::asset_manager::rm_mesh(std::size_t hash)
{
    auto it = sm_meshes.find(hash);
    if(it != sm_meshes.end())
        sm_meshes.erase(it);
    else
        throw s3gl::exception("[ERROR]: Hashed object does not exist (rm_mesh)\n");
}
