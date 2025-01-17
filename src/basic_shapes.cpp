#include "basic_shapes.hpp"
#include "GL/glew.h"
#include "window.hpp"
namespace mGLu
{
    
    static const char *rectVS = R"DENOM(
    out vec2 inRectPos;
    out flat vec2 rectSize;
    out flat vec3 edgeColor;
    out flat vec3 rectColor;
    out flat vec2 edgeWidth;
    out flat float alpha;
    void main()
    {
        rectSize = posNSize.zw;
        inRectPos = vertPos * rectSize;
        gl_Position = vec4(inRectPos + posNSize.xy, 0, 1);

        edgeColor = edgeCol;
        rectColor = color.rgb;
        alpha = color.a;
        edgeWidth = edgeSize;
    }
    )DENOM",
    *rectFS = R"DENOM(
    in vec2 inRectPos;
    in flat vec2 rectSize;
    in flat vec3 edgeColor;
    in flat vec3 rectColor;
    in flat vec2 edgeWidth;
    in flat float alpha;

    out vec4 fragColor;
    void main()
    {
        if( abs(inRectPos.x) <= edgeWidth.x || abs(inRectPos.y) <= edgeWidth.y || abs(rectSize.x - inRectPos.x) <= edgeWidth.x || abs(rectSize.y - inRectPos.y) <= edgeWidth.y)
            fragColor = vec4(edgeColor, alpha);
        else
            fragColor = vec4(rectColor, alpha);
    }
    )DENOM";

    static const char *pxRectVS = R"DENOM(
    out vec2 inRectPos;
    out flat vec2 rectSize;
    out flat vec3 edgeColor;
    out flat vec3 rectColor;
    out flat vec2 edgeWidth;
    out flat float alpha;
    void main()
    {
        rectSize = posNSize.zw  * vec2(1, -1);
        inRectPos = vertPos * rectSize;
        vec2 pixelPos = (inRectPos + posNSize.xy * vec2(1, -1));
        gl_Position = vec4(pixelPos/mGLuGlobal.viewportSize * 2 + vec2(-1.f, 1.f), 0, 1);

        edgeColor = edgeCol;
        rectColor = color.rgb;
        alpha = color.a;
        edgeWidth = edgeSize;
    }
    )DENOM",
    *pxRectFS = R"DENOM(
    in vec2 inRectPos;
    in flat vec2 rectSize;
    in flat vec3 edgeColor;
    in flat vec3 rectColor;
    in flat vec2 edgeWidth;
    in flat float alpha;

    out vec4 fragColor;
    void main()
    {
        if( abs(inRectPos.x) <= edgeWidth.x || abs(inRectPos.y) <= edgeWidth.y || abs(rectSize.x - inRectPos.x) <= edgeWidth.x || abs(rectSize.y - inRectPos.y) <= edgeWidth.y)
            fragColor = vec4(edgeColor, alpha);
        else
            fragColor = vec4(rectColor, alpha);
    }
    )DENOM";
    RectDrawable::RectDrawable(const Window* window):
        singleRectDrawBuffer(1, (RectangleData*)nullptr, GL_DYNAMIC_STORAGE_BIT)
    {
        VAO rectVAO;
        vao = rectVAO;
        GLuint  posNSizeBinding = rectVAO.AddAttrib(GL_FLOAT, 4, "posNSize", 1),
                colorBinding = rectVAO.AddAttrib(GL_FLOAT, 4, "color", 1),
                edgeColBinding = rectVAO.AddAttrib(GL_FLOAT, 3, "edgeCol", 1),
                edgeSizeBinding = rectVAO.AddAttrib(GL_FLOAT, 2, "edgeSize", 1),
                vertPosBinding = rectVAO.AddAttrib(GL_FLOAT, 2, "vertPos");
        
        buffers.resize(2);

        
        SetBinding(posNSizeBinding, 0, offsetof(RectangleData, pos), sizeof(RectangleData));
        SetBinding(colorBinding, 0, offsetof(RectangleData, color), sizeof(RectangleData));
        SetBinding(edgeColBinding, 0, offsetof(RectangleData, edgeCol), sizeof(RectangleData));
        SetBinding(edgeSizeBinding, 0, offsetof(RectangleData, edgeSize), sizeof(RectangleData));
        SetBinding(vertPosBinding, 1, 0, sizeof(glm::vec2));
        
        buffers[1] = FixedBuffer(sizeof(verts), verts);
        //puts((rectVAO.GetShaderPrefix() + rectVS).c_str());
        shader = Shader(*window, (rectVAO.GetShaderPrefix() + rectVS).c_str(), rectFS);
    }
    void RectDrawable::Draw(const RectangleSet& rects)
    {
        buffers[0] = rects.buffer;
        DrawInstanced(buffers[0].GetSize()/sizeof(RectangleData), 4, 0, GL_TRIANGLE_FAN);
    }
    void RectDrawable::Draw(const Rectangle& rect)
    {
        singleRectDrawBuffer.SetData(0, 1, &rect.data);
        buffers[0] = singleRectDrawBuffer;
        DrawInstanced(1, 4, 0, GL_TRIANGLE_FAN);
    }

    Rectangle::Rectangle(RectDrawable* _handler, glm::vec2 _pos, glm::vec2 _size, glm::vec4 _color, glm::vec3 _edgeColor, glm::vec2 _edgeWidth):
        handler(_handler),
        data{_pos, _size, _color, _edgeColor, _edgeWidth}
    {
        
    }
    
    void Rectangle::Draw()
    {
        handler->Draw(*this);
    }
    RectangleSet::RectangleSet(RectDrawable* _handler):
        handler(_handler),
        buffer(0, nullptr, GL_DYNAMIC_DRAW)
    {

    }
    void RectangleSet::UpdateBuffer()
    {
        buffer.ReallocBuffer(rectangles.size(), rectangles.data(), GL_DYNAMIC_DRAW);
    }
    void RectangleSet::Draw()
    {
        handler->Draw(*this);
    }

    PixelRectDrawable::PixelRectDrawable(const Window* window):
        singleRectDrawBuffer(1, (RectangleData*)nullptr, GL_DYNAMIC_STORAGE_BIT)
    {
        VAO rectVAO;
        vao = rectVAO;
        GLuint  posNSizeBinding = rectVAO.AddAttrib(GL_FLOAT, 4, "posNSize", 1),
                colorBinding = rectVAO.AddAttrib(GL_FLOAT, 4, "color", 1),
                edgeColBinding = rectVAO.AddAttrib(GL_FLOAT, 3, "edgeCol", 1),
                edgeSizeBinding = rectVAO.AddAttrib(GL_FLOAT, 2, "edgeSize", 1),
                vertPosBinding = rectVAO.AddAttrib(GL_FLOAT, 2, "vertPos");
        
        buffers.resize(2);

        
        SetBinding(posNSizeBinding, 0, offsetof(RectangleData, pos), sizeof(RectangleData));
        SetBinding(colorBinding, 0, offsetof(RectangleData, color), sizeof(RectangleData));
        SetBinding(edgeColBinding, 0, offsetof(RectangleData, edgeCol), sizeof(RectangleData));
        SetBinding(edgeSizeBinding, 0, offsetof(RectangleData, edgeSize), sizeof(RectangleData));
        SetBinding(vertPosBinding, 1, 0, sizeof(glm::vec2));
        
        buffers[1] = FixedBuffer(sizeof(verts), verts);
        //puts((rectVAO.GetShaderPrefix() + rectVS).c_str());
        shader = Shader(*window, (rectVAO.GetShaderPrefix() + pxRectVS).c_str(), pxRectFS);
    }
    void PixelRectDrawable::Draw(const PixelRectangleSet& rects)
    {
        buffers[0] = rects.buffer;
        DrawInstanced(buffers[0].GetSize()/sizeof(RectangleData), 4, 0, GL_TRIANGLE_FAN);
    }
    void PixelRectDrawable::Draw(const PixelRectangle& rect)
    {
        singleRectDrawBuffer.SetData(0, 1, &rect.data);
        buffers[0] = singleRectDrawBuffer;
        DrawInstanced(1, 4, 0, GL_TRIANGLE_FAN);
    }

    PixelRectangle::PixelRectangle(PixelRectDrawable* _handler, glm::vec2 _pos, glm::vec2 _size, glm::vec4 _color, glm::vec3 _edgeColor, glm::vec2 _edgeWidth):
        handler(_handler),
        data{_pos, _size, _color, _edgeColor, _edgeWidth}
    {
        
    }
    
    void PixelRectangle::Draw()
    {
        handler->Draw(*this);
    }
    PixelRectangleSet::PixelRectangleSet(PixelRectDrawable* _handler):
        handler(_handler),
        buffer(0, nullptr, GL_DYNAMIC_DRAW)
    {

    }
    void PixelRectangleSet::UpdateBuffer()
    {
        buffer.ReallocBuffer(rectangles.size(), rectangles.data(), GL_DYNAMIC_DRAW);
    }
    void PixelRectangleSet::Draw()
    {
        handler->Draw(*this);
    }
};
