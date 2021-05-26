#version 320 es

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 vertTextureCoord;

vec3 animatedPosition;

out vec2 TexCoord;
uniform float Time;
uniform float flag_width;

void main()
{
    animatedPosition = position;
    if (position.x > -flag_width/3.0f) {
        float len = (0.12 + position.x) * (0.12 + position.x); 
        float shift = sin(Time*0.08 + (position.x + flag_width/3.0f)*15.0)*len;
        animatedPosition.y += shift;
        animatedPosition.x += shift/2.0f;
    }
    gl_Position = vec4(animatedPosition.xyz, 1.0);
    TexCoord = vertTextureCoord;
}