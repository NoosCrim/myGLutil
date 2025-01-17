#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>
#include "myGLutil.hpp"
static const char *vsCode = R"DENOM(
out vec2 vecUV;
out vec3 vecPos;
void main()
{
/*
    if(gl_VertexID % 4 == 0)
        gl_Position = vec4(0,0,0,1);
    if(gl_VertexID % 4 == 1)
        gl_Position = vec4(1,0,0,1);
    if(gl_VertexID % 4 == 2)
        gl_Position = vec4(1,1,0,1);
    if(gl_VertexID % 4 == 3)
        gl_Position = vec4(0,1,0,1);
*/
    gl_Position = vec4(inPos,1);
    vecUV = inUV;
}
)DENOM";
static const char *fsCode = R"DENOM(
in vec2 vecUV;
in vec3 vecPos;
out vec4 outColor;
void main()
{
    outColor = vec4(vecUV, 0, 1);
}
)DENOM";
glm::vec3 posSplit[4] = {
    {-0.5f, -0.5f, 0.5f},
    { 0.5f, -0.5f, 0.5f},
    { 0.5f,  0.5f, 0.5f},
    {-0.5f,  0.5f, 0.5f}
};
glm::vec2 uvSplit[4] = 
{
    {0.f, 0.f},
    {1.f, 0.f},
    {1.f, 1.f},
    {0.f, 1.f}
};
struct vertexInterleaved
{
    glm::vec3 pos;
    glm::vec2 uv;
};
vertexInterleaved verticesInterleaved[4] = 
{
    {posSplit[0], uvSplit[0]},
    {posSplit[1], uvSplit[1]},
    {posSplit[2], uvSplit[2]},
    {posSplit[3], uvSplit[3]}
};
GLuint indices[6] = 
{
    0,1,2,
    0,2,3
};
class TestWindow : public mGLu::DefaultWindow
{
    mGLu::VAO testVAO;
    mGLu::FixedBuffer interleavedVBO, posVBO, uvVBO, EBO;
    mGLu::Shader testShader;
    mGLu::Drawable splitDrawable, interleavedDrawable;
    mGLu::MouseCallbackFlex testMouseCallback;
public:
    bool useSplit = true;
    TestWindow(int _width, int _height, bool fullscreen):
        mGLu::DefaultWindow(_width, _height, "title", fullscreen, 4, 2, true),
        testMouseCallback(&mouseInput, [](int button, bool isDown, glm::vec2 mousePos){printf("button: %d, state: %s\n", button, isDown?"down":"up");})
    {
        
    }
    virtual void Start() override
    {
        glDisable(GL_CULL_FACE);
        interleavedVBO = mGLu::FixedBuffer(4, verticesInterleaved);
        posVBO = mGLu::FixedBuffer(4, posSplit);
        uvVBO = mGLu::FixedBuffer(4, uvSplit);
        EBO = mGLu::FixedBuffer(6, indices);

        splitDrawable.vao = interleavedDrawable.vao = testVAO;
        
        GLuint posBinding = testVAO.AddAttrib(GL_FLOAT, 3, "inPos");
        GLuint uvBinding  = testVAO.AddAttrib(GL_FLOAT, 2, "inUV");
        
        splitDrawable.buffers.push_back(posVBO);
        splitDrawable.buffers.push_back(uvVBO);
        interleavedDrawable.buffers.push_back(interleavedVBO);
        
        splitDrawable.SetBinding(posBinding, 0, 0, sizeof(glm::vec3));
        splitDrawable.SetBinding(uvBinding, 1, 0, sizeof(glm::vec2));
        interleavedDrawable.SetBinding(posBinding, 0, offsetof(vertexInterleaved, pos), sizeof(vertexInterleaved));
        interleavedDrawable.SetBinding(uvBinding, 0, offsetof(vertexInterleaved, uv), sizeof(vertexInterleaved));
        
        splitDrawable.indexBuffer = interleavedDrawable.indexBuffer = EBO;
        
        std::string shaderCode = (testVAO.GetShaderPrefix() + vsCode).c_str();

        testShader = mGLu::Shader(*this, shaderCode.c_str(), fsCode);
        splitDrawable.shader = interleavedDrawable.shader = testShader;
    }
    virtual void Update() override
    {
        glClear(GL_COLOR_BUFFER_BIT);
        if(useSplit)
        {
            splitDrawable.DrawIndexed();
        }
        else
        {
            interleavedDrawable.DrawIndexed();
        }
        //printf("%d\n", mouseInput.GetMouseButtonState(GLFW_MOUSE_BUTTON_LEFT));
        if(mouseInput.GetMouseButtonState(GLFW_MOUSE_BUTTON_LEFT)==mGLu::UNPRESS)
        {
            glm::vec2 clickPos = mouseInput.GetMousePos();
            printf("click pos: (%f; %f)\n", clickPos.x, clickPos.y);
        }
    }
};
int main()
{
    TestWindow testWindow(800, 800, false);
    testWindow.useSplit = false;
    testWindow.StartMainLoop();
    return 0;
}