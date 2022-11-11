#version 420 core

in vec2 TexCoord;
in vec4 color;
uniform sampler2D widget_texture;
uniform bool texture_is_used;
out vec4 FragColor;

void main()
{
    FragColor = color;
    if(texture_is_used)
        FragColor += texture(widget_texture, TexCoord);
}