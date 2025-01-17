#include "key_input.hpp"
#include <cstdio>
namespace mGLu
{
    KeyCallbackHandler::KeyCallbackHandler()
    {
        for(int i = 0; i <= GLFW_KEY_LAST; i++)
            keyStates[i] = UP;
    }
    void KeyCallbackHandler::UpdateKeyState(int key, int glfw_key_action)
    {
        if(keyStates[key] == UP)
        {
            if(glfw_key_action)
                keyStates[key] = PRESS;
            else
                keyStates[key] = UP;
        }
        else
        {
            if(!glfw_key_action)
            {
                keyStates[key] = UNPRESS;
                _1frameStates.push_back(key);
            }
            else
                keyStates[key] = DOWN;
        }
    }
    void KeyCallbackHandler::HandleKeyInput(int key, int glfw_key_action)
    {
        UpdateKeyState(key, glfw_key_action);
        for(KeyCallback *callback : keyCallbacks)
            callback->KeyCallbackFunc(key, keyStates[key]);
    }
    void KeyCallbackHandler::ClearUnpressedKeys()
    {
        for(int unpressed_key : _1frameStates)
        {
            for(int unpressed_button : _1frameStates)
        {
            switch(keyStates[unpressed_button])
            {
            case DOWN:
            case UP:
                continue;
            case UNPRESS:
                keyStates[unpressed_button] = UP;
                break;
            case PRESS:
                keyStates[unpressed_button] = DOWN;    
                break;
            }
        }
        _1frameStates.clear();
        }
        _1frameStates.clear();
    }
    InputState KeyCallbackHandler::GetKeyState(int key) const
    {
        return keyStates[key];
    }
    void KeyCallback::SetHandler(KeyCallbackHandler *handler)
    {
        if(myHandler)
        {
            myHandler->keyCallbacks[inHandlerIndex] = myHandler->keyCallbacks.back();
            myHandler->keyCallbacks[inHandlerIndex]->inHandlerIndex = inHandlerIndex;
            myHandler->keyCallbacks.pop_back();
        }
        myHandler = handler;
        if(myHandler)
        {
            inHandlerIndex = handler->keyCallbacks.size();
            myHandler->keyCallbacks.push_back(this);
        }
    }
    KeyCallback::KeyCallback(KeyCallbackHandler *handler)
    {
        SetHandler(handler);
    }
    KeyCallback::~KeyCallback()
    {
        if(myHandler)
        {
            myHandler->keyCallbacks[inHandlerIndex] = myHandler->keyCallbacks.back();
            myHandler->keyCallbacks[inHandlerIndex]->inHandlerIndex = inHandlerIndex;
            myHandler->keyCallbacks.pop_back();
        }
    }
    KeyCallbackFlex::KeyCallbackFlex(KeyCallbackHandler *handler, KeyCallBackFunction callbackFunc):
        KeyCallback(handler),
        myCallbackFunc(callbackFunc)
    {

    }
}
