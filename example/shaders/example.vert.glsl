#version 460

in vec3 inPos;
in vec2 inUV;

out vec2 vecUV;
out vec3 vecPos;
void main()
{
    gl_Position = vec4(inPos,1);
    vecUV = inUV;
}