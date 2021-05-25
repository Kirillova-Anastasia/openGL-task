#version 320 es

out mediump vec4 fragColor;

in mediump vec3 outColor;
in mediump vec2 TexCoord;

uniform mediump vec3 color;
uniform sampler2D ourTexture;

void main()
{
    fragColor = texture(ourTexture, TexCoord);
}