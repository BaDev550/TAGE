#version 450 core
#include "Common.glsl"

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in ivec4 aBoneIDs; 
layout(location = 6) in vec4 aWeights;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec3 Tangent; 
out vec3 Bitangent;
out vec4 FragPosLightSpace;
out mat3 TBN;

uniform mat4 u_Model;
uniform mat4 viewProj;
uniform mat4 u_LightSpaceMatrix;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(aBoneIDs[i] == -1) 
            continue;
        if(aBoneIDs[i] >=MAX_BONES) 
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[aBoneIDs[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * aWeights[i];
        vec3 localNormal = mat3(finalBonesMatrices[aBoneIDs[i]]) * aNormal;
    }

    FragPos = vec3(u_Model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    mat3 u_NormalMatrix = mat3(transpose(inverse(u_Model)));
    Normal = u_NormalMatrix * aNormal;
    Tangent = u_NormalMatrix * aTangent;
    Bitangent = u_NormalMatrix * aBitangent;

    vec3 T = normalize(vec3(u_Model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(u_Model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(u_Model * vec4(aNormal,    0.0)));
    TBN = transpose(mat3(T, B, N));   

    FragPosLightSpace = u_LightSpaceMatrix * vec4(FragPos, 1.0);

    gl_Position = viewProj * vec4(FragPos, 1.0f);
}
