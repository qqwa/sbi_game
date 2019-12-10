#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture_diffuse;
uniform vec4 materialColor;

void main()
{
    color = materialColor * 3 + texture(texture_diffuse, TexCoord) * vec4(0.5, 0.5, 0.5, 1);
    color.w = materialColor.w;
    //color = vec4(TexCoord.xy, 1.0f, 1.0f);
}