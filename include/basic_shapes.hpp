#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "drawable.hpp"
namespace mGLu
{
    class Rectangle;
    class RectangleSet;
    class RectDrawable : private Drawable
    {
        static constexpr glm::vec2 verts[4] = 
        {
            {0.f, 0.f},
            {1.f, 0.f},
            {1.f, 1.f},
            {0.f, 1.f}
        };
        FixedBuffer singleRectDrawBuffer;
    public:
        RectDrawable(const Window* window);
        void Draw(const RectangleSet& rects);
        void Draw(const Rectangle& rect);
    };
    struct RectangleData
    {
        glm::vec2 pos;
        glm::vec2 size;
        glm::vec4 color;
        glm::vec3 edgeCol;
        glm::vec2 edgeSize;
    };
    class Rectangle
    {
        friend class RectDrawable;
        friend class RectangleSet;
        RectDrawable* handler;
    public:
        RectangleData data;
        Rectangle(RectDrawable* handler, glm::vec2 pos, glm::vec2 size, glm::vec4 color = {1.f, 1.f, 1.f, 1.f}, glm::vec3 edgeColor = {0.f, 0.f, 0.f}, glm::vec2 edgeWidth = {0.f,0.f});
        void Draw();
    };
    class RectangleSet
    {
        friend class RectDrawable;
        FlexBuffer buffer;
        RectDrawable* handler;
    public:
        std::vector<RectangleData> rectangles;
        RectangleSet(RectDrawable* handler);
        void UpdateBuffer();
        void Draw();
    };


    class PixelRectangle;
    class PixelRectangleSet;
    class PixelRectDrawable : private Drawable
    {
        static constexpr glm::vec2 verts[4] = 
        {
            {0.f, 0.f},
            {1.f, 0.f},
            {1.f, 1.f},
            {0.f, 1.f}
        };
        FixedBuffer singleRectDrawBuffer;
    public:
        PixelRectDrawable(const Window* window);
        void Draw(const PixelRectangleSet& rects);
        void Draw(const PixelRectangle& rect);
    };
    class PixelRectangle
    {
        friend class PixelRectDrawable;
        friend class PixelRectangleSet;
        PixelRectDrawable* handler;
    public:
        RectangleData data;
        PixelRectangle(PixelRectDrawable* handler, glm::vec2 pos, glm::vec2 size, glm::vec4 color = {1.f, 1.f, 1.f, 1.f}, glm::vec3 edgeColor = {0.f, 0.f, 0.f}, glm::vec2 edgeWidth = {0.f,0.f});
        void Draw();
    };
    class PixelRectangleSet
    {
        friend class PixelRectDrawable;
        FlexBuffer buffer;
        PixelRectDrawable* handler;
    public:
        std::vector<RectangleData> rectangles;
        PixelRectangleSet(PixelRectDrawable* handler);
        void UpdateBuffer();
        void Draw();
    };


    struct BasicShapeHandlers
    {
        BasicShapeHandlers(const Window* window):
            rectangle(window),
            pixelRectangle(window)
        {

        }
        RectDrawable rectangle;
        PixelRectDrawable pixelRectangle;
    };
}