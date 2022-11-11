#version 420 core

layout (location = 0) in vec2 pos;  
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec4 color_;

uniform mat4 projection;
out vec2 TexCoord;
out vec4 color;

void main()
{
    color = color_;
    TexCoord = texCoord;
    gl_Position = projection * vec4(pos, 0., 1.0);
}