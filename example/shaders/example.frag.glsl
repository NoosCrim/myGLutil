#version 460

in vec2 vecUV;
in vec3 vecPos;
out vec4 outColor;
void main()
{
    outColor = vec4(vecUV, 0, 1);
}