#include "manager.h"

s3gl::asset_manager& s3gl::asset_manager::get()
{
    return sm_instance;
}

std::size_t s3gl::asset_manager::new_mesh(const std::string& objname, const std::string& objfpath, const std::string& texfpath, const std::string& fragfpath, const std::string& vertfpath, int tex_unit, const glm::vec3& pos)
{
    // create hash for accsesing obj later
    std::size_t hash = sm_hasher(objname);
    sm_mesh[hash] = std::make_unique<s3gl::mesh>(objfpath, texfpath, fragfpath, vertfpath, tex_unit, pos);
    sm_mesh.at(hash)->link_atribute({0, 1, 2}, {3, 2, 3}, 8 * sizeof(float), {(void*)0, (void*)(3 * sizeof(float)), (void*)(6 * sizeof(float))});
    sm_mesh.at(hash)->set_tex_flags(s3gl::MESH_TEX_PRESET_1);
    return hash;
}

const s3gl::mesh& s3gl::asset_manager::get_mesh(std::size_t hash)
{
    if(sm_mesh.find(hash) != sm_mesh.end())
        return *sm_mesh.at(hash);
    else
        throw s3gl::exception("[ERROR]: Hashed object does not exist (get_mesh)\n");
}

void s3gl::asset_manager::rm_mesh(std::size_t hash)
{
    auto it = sm_mesh.find(hash);
    if(it != sm_mesh.end())
        sm_mesh.erase(it);
    else
        throw s3gl::exception("[ERROR]: Hashed object does not exist (rm_mesh)\n");
}
