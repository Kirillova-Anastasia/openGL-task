#version 320 es

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertTextureCoord;

out vec3 outColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position.xyz, 1.0);
    outColor = vertexColor;
    TexCoord = vertTextureCoord;
}