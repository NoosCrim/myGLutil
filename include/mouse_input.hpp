#pragma once
#include <functional>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "input.hpp"
namespace mGLu
{
    typedef std::function<void(int button, bool isDown, glm::vec2 mousePos)> MouseCallBackFunction;
    class MouseCallback;
    class MouseCallbackHandler
    {
        friend class MouseCallback;
        glm::vec2 mousePos, mouseScroll, mouseMove;
        void UpdateMouseButtonState(int button, int glfw_button_action);
    protected:
        std::vector<MouseCallback*> mouseCallbacks;
        std::vector<int> _1frameStates;
        InputState mouseStates[GLFW_MOUSE_BUTTON_LAST+1];
    public:
        void UpdateMousePos(glm::vec2 mousePos);
        void UpdateMouseScroll(glm::vec2 mouseScroll);
        MouseCallbackHandler();
        void HandleMouseInput(int button, int glfw_button_action);
        void Clear1FrameStates();
        InputState GetMouseButtonState(int mouseButton) const;
        glm::vec2 GetMousePos() const;
        glm::vec2 GetMouseMove() const;
        glm::vec2 GetMouseScroll() const;
    };
   
    class MouseCallback
    {
        MouseCallbackHandler *myHandler = nullptr;
        unsigned int inHandlerIndex = 0;
    public:
        virtual void MouseCallbackFunc(int button, bool isDown, glm::vec2 mousePos) = 0;
        void SetHandler(MouseCallbackHandler *handler);
        MouseCallback(MouseCallbackHandler *handler);
        virtual ~MouseCallback();
    };

    class MouseCallbackFlex : public MouseCallback
    {
    public:
        MouseCallBackFunction myCallbackFunc;   
        MouseCallbackFlex(MouseCallbackHandler *handler, MouseCallBackFunction callbackFunc);
        virtual void MouseCallbackFunc(int button, bool isDown, glm::vec2 mousePos) final override
        {
            myCallbackFunc(button, isDown, mousePos);
        }
    };
}