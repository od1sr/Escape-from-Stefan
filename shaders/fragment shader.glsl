#version 420 core

const int max_textures_num = 8;

struct Material 
{
    sampler2D diffuse_maps[max_textures_num];
    sampler2D specular_maps[max_textures_num];
    int diff_textures_num;
    int spec_textures_num;
    int shininess;
};

struct LightForce
{
    float constant;
    float linear;
    float quadratic;
};

struct Light
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight
{ 
    vec3 pos;
    Light settings;
    LightForce force;
};

struct DirectionalLight
{
    vec3 direction;
    Light settings;
};

struct ProjectLight
{
    float inside_angle;
    float outside_angle;
    vec3 pos;
    vec3 direction;
    Light settings;
    LightForce force;
};
in vec2 TexCoord;
out vec4 FragColor;
in vec3 normal;
in vec3 frag_pos;
in vec4 diffuse_color;
in vec4 specular_color;
uniform vec3 camera_pos;
uniform Material material;
uniform DirectionalLight d_light;
uniform bool directional_light_is_using;
const int max_p_light_num = 10;
const int max_prj_light_num = 10;
uniform PointLight p_lights[max_p_light_num];
uniform int p_light_num;
uniform ProjectLight prj_lights[max_prj_light_num];
uniform int prj_light_num;

Light calculateLight(Light light, vec3 lightDir);
Light calculatePointLight(PointLight light);
Light calculateProjecttLight(ProjectLight light);

void main()
{   
    vec4 result = vec4(0.);
    if (directional_light_is_using)
    {
        Light l = calculateLight(d_light.settings, d_light.direction);
        result += l.ambient + l.specular + l.diffuse;
    }
    for(int i = 0; i < p_light_num; ++i)
    {
        Light l = calculatePointLight(p_lights[i]);
        result += l.ambient + l.diffuse + l.specular;
    }
    for(int i = 0; i < prj_light_num; ++i)
    {
        Light l = calculateProjecttLight(prj_lights[i]);
        result += l.ambient + l.diffuse + l.specular;
    }
    FragColor = result;
}


Light calculateLight(Light light, vec3 lightDir)
{
    Light result;
    vec3 reflectDir, norm, viewDir;
    float diff;
    vec4 material_diffuse = diffuse_color;
    vec4 material_specular = specular_color;
    for(int i = 0; i < material.diff_textures_num && i < max_textures_num; ++i)
        material_diffuse += texture(material.diffuse_maps[i], TexCoord);
    for(int i = 0; i < material.spec_textures_num && i < max_textures_num; ++i)
        material_specular += texture(material.specular_maps[i], TexCoord);
    result.ambient = light.ambient * material_diffuse;
    norm = normalize(normal);
    diff = max(dot(norm, lightDir), 0);
    result.diffuse = light.diffuse * material_diffuse * diff;  
    viewDir = normalize(camera_pos - frag_pos);
    reflectDir = reflect(-lightDir, norm);
    result.specular = vec4(0.);
    if(diff > 0)
    { 
        float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
        result.specular = light.specular * spec * material_specular;  
    }
    return result;
}

Light calculatePointLight(PointLight light)
{
    Light result = calculateLight(light.settings, normalize(light.pos - frag_pos));
    float d, attenuation;
    d = length(light.pos - frag_pos);
    attenuation = 1.0f / (
        light.force.constant + light.force.linear*d + light.force.quadratic*d*d);
    result.ambient *= attenuation;
    result.specular *= attenuation;
    result.diffuse *= attenuation;
    return result;
}

Light calculateProjecttLight(ProjectLight light)
{
    Light result;
    float angle = acos(dot(light.direction, normalize(frag_pos - light.pos)));
    if (angle > light.outside_angle)
        result.specular = result.diffuse = result.ambient = vec4(0.);
    else 
    {
        PointLight p_light;
        p_light.pos = light.pos;
        p_light.force = light.force;
        p_light.settings = light.settings;
        result = calculatePointLight(p_light);
        if (angle > light.inside_angle)
        {
            float rad_diff = light.outside_angle - light.inside_angle;
            float q = (rad_diff - (angle-light.inside_angle)) / rad_diff; 
            result.diffuse *= q;
            result.specular *= q;
        }
    }
    return result;
}
