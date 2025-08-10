#pragma once
#include <functional>
#include <GLFW/glfw3.h>
#include <vector>
#include "input.hpp"
namespace mGLu
{
    using KeyCallBackFunction = std::function<void(int key, bool isDown)>;
    class KeyCallback;
    class KeyCallbackHandler
    {
        friend class KeyCallback;
        void UpdateKeyState(int key, int glfw_key_action);
    protected:
        std::vector<KeyCallback*> keyCallbacks;
        std::vector<int> _1frameStates;
        InputState keyStates[GLFW_KEY_LAST+1];
    public:
        KeyCallbackHandler();
        void HandleKeyInput(int key, int glfw_key_action);
        void ClearUnpressedKeys();
        InputState GetKeyState(int key) const;
    };
    
    class KeyCallback
    {
        KeyCallbackHandler *myHandler = nullptr;
        unsigned int inHandlerIndex = 0;
    public:
        void SetHandler(KeyCallbackHandler *handler);
        KeyCallback(KeyCallbackHandler *handler = nullptr);
        virtual void KeyCallbackFunc(int key, bool isDown) = 0;
        virtual ~KeyCallback();
    };

    class KeyCallbackFlex : public KeyCallback
    {
    public:
        KeyCallBackFunction myCallbackFunc;
        KeyCallbackFlex(KeyCallbackHandler *handler, KeyCallBackFunction callbackFunc);
        virtual void KeyCallbackFunc(int key, bool isDown) final override
        {
            myCallbackFunc(key, isDown);
        }
    };
}