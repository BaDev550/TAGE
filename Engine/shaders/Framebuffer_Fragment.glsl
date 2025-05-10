#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomBlur;
uniform float u_Exposure;
uniform bool u_BloomEnabled;

void main()
{
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;

    if (u_BloomEnabled)
        hdrColor += bloomColor;

    vec3 toneMapped = hdrColor * u_Exposure;

    FragColor = vec4(toneMapped, 1.0);
} 