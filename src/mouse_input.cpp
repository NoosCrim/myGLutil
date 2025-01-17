#include "mouse_input.hpp"
namespace mGLu
{
    MouseCallbackHandler::MouseCallbackHandler()
    {
        for(int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++)
            mouseStates[i] = UP;
    }
    void MouseCallbackHandler::UpdateMouseButtonState(int button, int glfw_button_action)
    {
        if(mouseStates[button] == UP)
        {
            if(glfw_button_action)
            {
                mouseStates[button] = PRESS;
                _1frameStates.push_back(button);
            }
        }
        else
        {
            if(!glfw_button_action)
            {
                mouseStates[button] = UNPRESS;
                _1frameStates.push_back(button);
            }
            else
                mouseStates[button] = DOWN;
        }
    }
    void MouseCallbackHandler::UpdateMousePos(glm::vec2 _mousePos)
    {
        mouseMove = _mousePos - mousePos;
        mousePos = _mousePos;
    }
    void MouseCallbackHandler::UpdateMouseScroll(glm::vec2 _mouseScroll)
    {
        mouseScroll = _mouseScroll;
    }
    void MouseCallbackHandler::HandleMouseInput(int button, int glfw_button_action)
    {
        UpdateMouseButtonState(button, glfw_button_action);
        for(MouseCallback* callback : mouseCallbacks)
        {
            callback->MouseCallbackFunc(button, glfw_button_action, mousePos);
        }
    }
    void MouseCallbackHandler::Clear1FrameStates()
    {
        for(int unpressed_button : _1frameStates)
        {
            switch(mouseStates[unpressed_button])
            {
            case DOWN:
            case UP:
                continue;
            case UNPRESS:
                mouseStates[unpressed_button] = UP;
                break;
            case PRESS:
                mouseStates[unpressed_button] = DOWN;    
                break;
            }
        }
        _1frameStates.clear();
    }
    InputState MouseCallbackHandler::GetMouseButtonState(int mouseButton) const
    {
        return mouseStates[mouseButton];
    }
    glm::vec2 MouseCallbackHandler::GetMousePos() const
    {
        return mousePos;
    }
    glm::vec2 MouseCallbackHandler::GetMouseMove() const
    {
        return mouseMove;
    }
    glm::vec2 MouseCallbackHandler::GetMouseScroll() const
    {
        return mouseScroll;
    }
    void MouseCallback::SetHandler(MouseCallbackHandler *handler)
    {
        if(myHandler)
        {
            myHandler->mouseCallbacks[inHandlerIndex] = myHandler->mouseCallbacks.back();
            myHandler->mouseCallbacks[inHandlerIndex]->inHandlerIndex = inHandlerIndex;
            myHandler->mouseCallbacks.pop_back();
        }
        myHandler = handler;
        if(myHandler)
        {
            inHandlerIndex = handler->mouseCallbacks.size();
            myHandler->mouseCallbacks.push_back(this);
        }
    }
    MouseCallback::MouseCallback(MouseCallbackHandler *handler)
    { 
        SetHandler(handler);
    }
    MouseCallbackFlex::MouseCallbackFlex(MouseCallbackHandler *handler, MouseCallBackFunction callbackFunc):
        MouseCallback(handler),
        myCallbackFunc(callbackFunc)
    {

    }
    MouseCallback::~MouseCallback()
    {
        SetHandler(nullptr);
    }
};
