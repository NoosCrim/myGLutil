#pragma once
#include "drawable.hpp"
#include "mouse_input.hpp"

namespace mGLu
{
    class UIelem : public TreeNode, public MouseCallback
    {
    private:
        virtual void MouseCallbackFunc(int button, bool isDown, glm::vec2 cursorPos) override;
        bool ClickCallback(int button, bool isDown, glm::vec2 cursorPos);
    protected:
        virtual void MyDraw() = 0;
        
    public:
        bool active = true;
        UIelem(UIelem* parent);
        UIelem(MouseCallbackHandler *handler);
        void Draw();
        virtual bool ClickCheck(glm::vec2 cursorPos) = 0;
        virtual bool OnClick(int mouseButton, bool isDown, glm::vec2 cursorPos) = 0; // returned bool is a value used to decide whether callbacks for lower layers should be called
    };
}