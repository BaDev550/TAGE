#ifndef COMMON_FRAGMENT_GLSL
#define COMMON_FRAGMENT_GLSL

struct Material {
    sampler2D u_DiffuseMap;
    sampler2D u_SpecularMap;
    sampler2D u_NormalMap;
    sampler2D u_MetallicMap;
    sampler2D u_RoughnessMap;
};

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjection;
    vec3 u_CameraPos;
};

#endif