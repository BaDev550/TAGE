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
uniform bool u_PlayAnimation;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 localPosition;
    vec3 localNormal;
    vec3 localTangent;
    vec3 localBitangent;

    if (u_PlayAnimation)
    {
        vec4 totalPosition = vec4(0.0);
        vec3 animatedNormal = vec3(0.0);
        vec3 animatedTangent = vec3(0.0);
        vec3 animatedBitangent = vec3(0.0);

        for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            if(aBoneIDs[i] < 0 || aBoneIDs[i] >= MAX_BONES)
                continue;

            mat4 boneTransform = finalBonesMatrices[aBoneIDs[i]];
            totalPosition     += boneTransform * vec4(aPos, 1.0) * aWeights[i];
            animatedNormal    += mat3(boneTransform) * aNormal    * aWeights[i];
            animatedTangent   += mat3(boneTransform) * aTangent   * aWeights[i];
            animatedBitangent += mat3(boneTransform) * aBitangent * aWeights[i];
        }

        localPosition = totalPosition;
        localNormal = animatedNormal;
        localTangent = animatedTangent;
        localBitangent = animatedBitangent;
    }
    else
    {
        localPosition = vec4(aPos, 1.0);
        localNormal = aNormal;
        localTangent = aTangent;
        localBitangent = aBitangent;
    }

    vec4 worldPosition = u_Model * localPosition;

    FragPos = vec3(worldPosition);
    TexCoord = aTexCoord;

    mat3 normalMatrix = mat3(transpose(inverse(u_Model)));
    Normal    = normalize(normalMatrix * localNormal);
    Tangent   = normalize(normalMatrix * localTangent);
    Bitangent = normalize(normalMatrix * localBitangent);

    TBN = transpose(mat3(
        normalize(vec3(u_Model * vec4(localTangent, 0.0))),
        normalize(vec3(u_Model * vec4(localBitangent, 0.0))),
        normalize(vec3(u_Model * vec4(localNormal, 0.0)))
    ));

    FragPosLightSpace = u_LightSpaceMatrix * worldPosition;

    gl_Position = viewProj * worldPosition;
}
