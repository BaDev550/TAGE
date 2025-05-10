#ifndef LIGHT_FRAGMENT_GLSL
#define LIGHT_FRAGMENT_GLSL

struct Light {
    vec3 Position;
    vec3 Color;
    float Intensity;
};

struct DirectionalLight {
	vec3 direction;
	vec3 color;
	float intensity;
};

vec3 CalculateLighting(vec3 normal, vec3 viewDir, vec3 fragPos, Light light, Material material, vec2 texCoords)
{
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    vec3 diffuse = diff * light.Color * texture(material.u_DiffuseMap, texCoords).rgb;
    vec3 specular = spec * light.Color * texture(material.u_SpecularMap, texCoords).rgb;

    return (diffuse + specular) * light.Intensity;
}

#endif