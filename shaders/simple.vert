#version 320 es

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 vertTextureCoord;

out vec2 TexCoord;
uniform float Time;
uniform float flag_width;
uniform float offsets[200];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 animatedPosition;
    animatedPosition = position;
    if (animatedPosition.x > -flag_width/3.0f) {
        float len = (0.12 + animatedPosition.x) * (0.12 + animatedPosition.x); 
        float shift = sin(Time *0.08 + (animatedPosition.x + flag_width/3.0f)*15.0)*len;
        animatedPosition.y += shift;
        animatedPosition.x += shift/2.0f;
    }

    vec2 offset = vec2(offsets[gl_InstanceID*2], offsets[gl_InstanceID*2+1]);
    animatedPosition.xz += offset;
    gl_Position = projection * view * model * vec4(animatedPosition.xyz, 1.0);
    TexCoord = vertTextureCoord;
}