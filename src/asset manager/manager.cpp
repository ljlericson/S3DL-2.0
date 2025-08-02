#include "manager.h"

s3gl::asset_manager& s3gl::asset_manager::get()
{
    return sm_instance;
}

std::size_t s3gl::asset_manager::new_mesh(const std::string& objname, const std::string& objfpath, const std::string& texfpath, const std::string& fragfpath, const std::string& vertfpath, int tex_unit, const glm::vec3& pos)
{
    // create the new mesh object
    s3gl::mesh* mesh = new s3gl::mesh(objfpath, texfpath, fragfpath, vertfpath, tex_unit, pos);
    mesh->link_atribute({0, 1, 2}, {3, 2, 3}, 8 * sizeof(float), {(void*)0, (void*)(3 * sizeof(float)), (void*)(6 * sizeof(float))});
    mesh->set_tex_flags(s3gl::MESH_TEX_PRESET_1);
    // wrap it in a shared pointer that 
    // will be freed in this scope in
    // this scope, making reference
    // count 1
    std::shared_ptr<s3gl::mesh> ptr = std::make_shared<s3gl::mesh>(*mesh);
    std::size_t hash = sm_hasher(objname);
    // add to obj map and return hash
    // used to accses obj
    sm_mesh[hash] = ptr;
    return hash;
}

const s3gl::mesh& s3gl::asset_manager::get_mesh(std::size_t hash)
{
    return *sm_mesh.at(hash);
}

void s3gl::asset_manager::rm_mesh(std::size_t hash)
{
    delete &sm_mesh.at(hash);
    sm_mesh.erase(sm_mesh.find(hash));
}
