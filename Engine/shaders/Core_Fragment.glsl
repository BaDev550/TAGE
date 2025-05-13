#version 450 core
#include "Common.glsl"
#include "Lighting.glsl"
#include "PBR_Fragment.glsl"

uniform vec3 u_CameraPosition;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;
in mat3 TBN;
out vec4 FragColor;

uniform Material u_Material;
uniform DirectionalLight u_Light;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(u_Light.direction - FragPos);
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.001);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    vec3 albedo = texture(u_Material.u_DiffuseMap, TexCoord).rgb;
    vec3 normal = getNormalFromMap(u_Material.u_NormalMap, TexCoord, Normal, FragPos);
    float metallic = texture(u_Material.u_MetallicMap, TexCoord).r;
    float roughness = texture(u_Material.u_RoughnessMap, TexCoord).r;

    vec3 viewDir =  normalize(u_CameraPosition - FragPos);
    vec3 lightDir = normalize(u_Light.direction - FragPos);

    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 pbr = PBR(albedo, metallic, roughness, normal, viewDir, lightDir, u_Light.color * u_Light.intensity);
    vec3 ambient = 0.6 * albedo;

    vec3 finalColor = (ambient + (1.0 - shadow) * pbr);
    FragColor = vec4(finalColor, 1.0);
}