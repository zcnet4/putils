#pragma once

#include <string>
#include <unordered_map>
#include <stack>

#include <CEGUI/WindowManager.h>

#include "fwd.hpp"

namespace pogre
{
    class CEGUIMenu
    {
    public:
        void init()
        {
            _windowManager = CEGUI::WindowManager::getSingletonPtr();
            _mouseCursor = &CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
            _mouseCursor->setVisible(false);
        }

        // Visibility
    public:
        bool isVisible() const noexcept
        {
            return _mouseCursor->isVisible();
        }

        void setVisible(bool visible)
        {
            _stack.top()->setVisible(visible);
            _mouseCursor->setVisible(visible);
        }

        void show() { setVisible(true); }

        void hide() { setVisible(false); }

        // Panel management
    public:
        template<typename String>
        CEGUI::Window &createPanel(String &&name)
        {
            auto &ret = *(_panels[name] = _windowManager->createWindow("DefaultWindow", name));
            ret.setVisible(false);
            return ret;
        }

        template<typename String>
        CEGUI::Window &getPanel(String &&name)
        {
            return *_panels.at(FWD(name));
        }

        // Stack management
    public:
        void push(CEGUI::Window &panel)
        {
            if (!_stack.empty())
                _stack.top()->setVisible(false);

            _stack.push(&panel);
            update();
        }

        void pop()
        {
            _stack.top()->setVisible(false);
            _stack.pop();
            update();
        }

        void clear()
        {
            while (!_stack.empty())
                pop();
        }

    public:
        CEGUI::WindowManager &getWindowManager() { return *_windowManager; }
        CEGUI::MouseCursor &getMouseCursor() { return *_mouseCursor; }

        // Helpers
    private:
        void update()
        {
            if (_stack.empty())
                return;

            if (_mouseCursor->isVisible())
                _stack.top()->setVisible(true);
            auto &guiContext = CEGUI::System::getSingleton().getDefaultGUIContext();
            guiContext.setRootWindow(_stack.top());
        }

        // Attributes
    private:
        CEGUI::WindowManager *_windowManager;
        CEGUI::MouseCursor *_mouseCursor;

    private:
        std::unordered_map<std::string, CEGUI::Window *> _panels;
        std::stack<CEGUI::Window *> _stack;
    };
}