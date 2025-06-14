#version 410 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 crnt_pos;

uniform int light_preset;
uniform sampler2D tex0;
uniform vec3 cam_pos;
uniform vec4 light_col;
uniform vec3 light_pos;

vec4 point_light()
{
    vec3 vlight = light_pos - crnt_pos;
    float dist = length(vlight);
    float a = 0.05f;
    float b = 0.04f;
    float intern = 1.0f / (a * dist * dist + b * dist + 1.0f);

    float ambient = 0.10f;


    vec3 normaln = normalize(normal);
    vec3 light_dir = normalize(light_pos - crnt_pos);
    float diffuse = max(dot(normaln, light_dir), 0.0f);

    float spec_light = 0.50f;
    vec3 view_dir = normalize(cam_pos - crnt_pos);
    vec3 reflection_dir = reflect(-light_dir, normaln);
    float spec_amout = pow(max(dot(view_dir, reflection_dir), 0.0f), 8);
    float specular = spec_amout * spec_light;
    float maxs = max(diffuse * intern, ambient);
    vec4 light_colv = light_col;
    if(maxs == 0.1f)
        light_colv = vec4(1.0f);
    return (texture(tex0, texCoord) * max(diffuse * intern, ambient) + specular * intern) * light_colv;
}

vec4 direct_light()
{
    float ambient = 0.10f;

    vec3 normaln = normalize(normal);
    vec3 light_dir = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuse = max(dot(normaln, light_dir), 0.0f);

    float spec_light = 0.50f;
    vec3 view_dir = normalize(cam_pos - crnt_pos);
    vec3 reflection_dir = reflect(-light_dir, normaln);
    float spec_amout = pow(max(dot(view_dir, reflection_dir), 0.0f), 8);
    float specular = spec_amout * spec_light;

    return (texture(tex0, texCoord) * max(diffuse, ambient) + specular) * light_col;
}

vec4 no_light()
{
    return texture(tex0, texCoord);
}

void main()
{
    if(light_preset == 0)
        FragColor = no_light();
    if(light_preset == 1)
        FragColor = direct_light();
    if(light_preset == 2)
        FragColor = point_light();
    FragColor.w = 1.0f;
}