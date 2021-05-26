#version 320 es

out mediump vec4 fragColor;

in mediump vec3 outColor;
in mediump vec2 TexCoord;

uniform mediump vec3 color;
uniform sampler2D ourTexture;

void main()
{
    if (TexCoord.x >= 0.0f) {
        fragColor = texture(ourTexture, TexCoord);
    } else if (TexCoord.x > -2.0f) {
        fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    } else {
        fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}