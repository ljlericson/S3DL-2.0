#include "manager.h"

s3gl::asset_manager s3gl::asset_manager::sm_instance;

s3gl::asset_manager::audio_buffer::audio_buffer(ALuint id)
    : id(id)  {  }

s3gl::asset_manager::audio_buffer::~audio_buffer()
{
    alDeleteBuffers(1, &id);
}

s3gl::asset_manager& s3gl::asset_manager::get()
{
    return sm_instance;
}

std::size_t s3gl::asset_manager::new_shad(std::string_view name_shad, std::string_view fpathVert, std::string_view fpathFrag)
{
    std::size_t hash = s3gl::fnv1a_hash((void*)name_shad.data(), name_shad.size());
    sm_shader_key_map[name_shad.data()] = hash;
    if (sm_shaders.contains(hash)) 
        throw s3gl::exception("Duplicate name in asset manager new shader\n");
    sm_shaders[hash] = std::make_unique<s3gl::shader>(fpathVert, fpathFrag);
    return hash;
}

std::size_t s3gl::asset_manager::new_tex(std::string_view name_tex, std::string_view fpath, std::size_t shad_hash)
{
    if(sm_textures.size() == 9) 
    {   std::cout << "[ERROR]: Texture slots are full (asset_mannager::new_tex())\n";   return -1;    }
    else
    {
        std::size_t hash = s3gl::fnv1a_hash((void*)name_tex.data(), name_tex.size());
        sm_texture_key_map[name_tex.data()] = hash;
        
        GLuint tex_slot = GL_TEXTURE0 + sm_textures.size();
        if (sm_textures.contains(hash)) 
            throw s3gl::exception("Duplicate name in asset manager new texture\n");
        sm_textures[hash] = std::make_unique<texture>(fpath.data(), tex_slot, GL_TEXTURE_2D);
        sm_textures[hash]->bind(sm_shaders[shad_hash]->id);
        return hash;
    }
}

std::size_t s3gl::asset_manager::new_mesh(std::string_view name_mesh, const std::string& objfpath, std::size_t shad_hash, std::size_t tex_hash, const glm::vec3& pos)
{
    // first check that the shader is valid
    if(sm_shaders.find(shad_hash) != sm_shaders.end() && sm_textures.find(tex_hash) != sm_textures.end())
    {
        std::size_t hash = s3gl::fnv1a_hash((void*)name_mesh.data(), name_mesh.size());
        sm_mesh_key_map[name_mesh.data()] = hash;

        if (sm_meshes.contains(hash)) 
            throw s3gl::exception("Duplicate name in asset manager new mesh\n");
        sm_meshes[hash] = std::make_unique<s3gl::mesh>(objfpath, *sm_shaders.at(shad_hash), *sm_textures.at(tex_hash), pos);
        sm_meshes.at(hash)->link_atribute({0, 1, 2}, {3, 2, 3}, 8 * sizeof(float), {(void*)0, (void*)(3 * sizeof(float)), (void*)(6 * sizeof(float))});
        sm_meshes.at(hash)->set_tex_flags(s3gl::MESH_TEX_PRESET_1);
        return hash;
    }
    else
    {   throw s3gl::exception("[ERROR]: Shader or texture hash entered is invalid (new_mesh)\n");  }
}

std::size_t s3gl::asset_manager::new_WAV_buffer(std::string_view name_audio, std::string_view fpathWAV)
{
    ALuint new_buffer;
    drwav_int16* pcm_data;
    unsigned int channels, sample_rate;
    drwav_uint64 total_PCM_frame_count;
    pcm_data = drwav_open_file_and_read_pcm_frames_s16(fpathWAV.data(), &channels, &sample_rate, &total_PCM_frame_count, nullptr);
    if (!pcm_data) throw s3gl::exception("Failed to load WAV");
    alGenBuffers(1, &new_buffer);
    alBufferData(new_buffer,
                channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
                pcm_data,
                total_PCM_frame_count * channels * sizeof(drwav_int16),
                sample_rate);
    
    // create hash for accsesing obj later
    std::size_t hash = s3gl::fnv1a_hash((void*)name_audio.data(), name_audio.size());
        sm_audio_key_map[name_audio.data()] = hash;

    if (sm_audio_buffers.contains(hash)) 
        throw s3gl::exception("Duplicate name in asset manager new audio\n");
    sm_audio_buffers[hash] = std::make_unique<audio_buffer>(new_buffer);
        
    drwav_free(pcm_data, nullptr);
    return hash;
}

s3gl::mesh& s3gl::asset_manager::get_mesh(std::size_t hash)
{
    if(sm_meshes.find(hash) != sm_meshes.end())
        return *sm_meshes.at(hash);
    else
        throw s3gl::exception("[ERROR]: Hashed object does not exist (get_mesh)\n");
}

const ALuint s3gl::asset_manager::get_WAV_buffer(std::size_t hash)
{
    if(sm_audio_buffers.find(hash) != sm_audio_buffers.end())
        return sm_audio_buffers.at(hash)->id;
    else
        throw s3gl::exception("[ERROR]: Hashed object does not exist (get_WAV_buffer)\n");
}

std::size_t s3gl::asset_manager::get_mesh_hash(std::string_view name)
{
    if(sm_mesh_key_map.contains(std::string(name))) { return sm_mesh_key_map.at(std::string((name))); }
    else { std::cout << "[WARNING]: No mesh hash found for given name (get_mesh_hash)\n"; return 0;}
}

std::size_t s3gl::asset_manager::get_shad_hash(std::string_view name)
{
    if(sm_shader_key_map.contains(std::string(name))) { return sm_shader_key_map.at(std::string(name)); }
    else { std::cout << "[WARNING]: No shader hash found for given name (get_shader_hash)\n"; return 0;}
}

std::size_t s3gl::asset_manager::get_tex_hash(std::string_view name)
{
    if(sm_texture_key_map.contains(std::string(name))) { return sm_texture_key_map.at(std::string(name)); }
    else { std::cout << "[WARNING]: No shader hash found for given name (get_texture_hash)\n"; return 0;}
}

std::size_t s3gl::asset_manager::get_WAV_hash (std::string_view name)
{
    if(sm_audio_key_map.contains(std::string(name))) { return sm_audio_key_map.at(std::string(name)); }
    else { std::cout << "[WARNING]: No shader hash found for given name (get_audio_hash)\n"; return 0;}
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

void s3gl::asset_manager::clear()
{
    sm_meshes.clear();
    sm_mesh_key_map.clear();

    sm_shaders.clear();
    sm_shader_key_map.clear();

    sm_textures.clear();
    sm_texture_key_map.clear();

    sm_audio_buffers.clear();
    sm_audio_key_map.clear();
}
