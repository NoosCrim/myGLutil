#include "ui_elem.hpp"
namespace mGLu
{
    void UIelem::MouseCallbackFunc(int button, bool isDown, glm::vec2 cursorPos)
    {
        if(active)
            ClickCallback(button, isDown, cursorPos);
    }
    bool UIelem::ClickCallback(int button, bool isDown, glm::vec2 cursorPos)
    {
        if(!active)
            return true;
        if(!ClickCheck(cursorPos))
            return true;

        bool passClick = true;

        for(TreeNode* child : GetChildren())
            passClick &= ((UIelem*)child)->ClickCallback(button, isDown, cursorPos);
        if(passClick)
            return OnClick(button, isDown, cursorPos);
        return false;
    }
    UIelem::UIelem(UIelem* parent):
        TreeNode(parent),
        MouseCallback(nullptr)
    {
        
    }
    UIelem::UIelem(MouseCallbackHandler *handler):
        TreeNode(nullptr),
        MouseCallback(handler)
    {
        
    }
    void UIelem::Draw()
    {
        if(!active)
            return;
        MyDraw();
        for(TreeNode* child : GetChildren())
            ((UIelem*)child)->Draw();
    }
}