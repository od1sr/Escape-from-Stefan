#version 420 core
layout (location = 0) in vec3 pos;  
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 surfase_normal;
layout (location = 3) in vec4 color_diffuse;
layout (location = 4) in vec4 color_specular;
out vec2 TexCoord;
out vec3 normal;
out vec3 frag_pos;
out vec4 diffuse_color;
out vec4 specular_color; 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    gl_Position = projection * view * model * vec4(pos, 1.0);
    frag_pos = (model * vec4(pos, 1.0)).xyz;
    TexCoord = texCoord;
    normal = normalize(mat3(transpose(inverse(model))) * surfase_normal);
    diffuse_color = color_diffuse;
    specular_color = color_specular;
}  