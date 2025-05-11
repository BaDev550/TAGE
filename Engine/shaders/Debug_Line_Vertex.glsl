#version 460 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

uniform mat4 u_ViewProj;

out vec3 v_Color;

void main()
{
    v_Color = aColor;
    gl_Position = u_ViewProj * vec4(aPosition, 1.0);
}
