#include "KeyManager.hpp"

namespace pogre
{
    namespace detail
    {
        void KeyManager::addKeyHandler(SDL_Keycode key, const KeyManager::KeyHandler &onPress,
                                       const KeyManager::KeyHandler &onRelease) noexcept
        {
            Lock _(_keyHandlersMutex);
            _keyHandlersToAdd[key].push_back({ onPress, onRelease });
        }

        void KeyManager::addKeyHandler(const AnyKeyHandler &onPress, const AnyKeyHandler &onRelease) noexcept
        {
            Lock _(_keyHandlersMutex);
            _anyKeyHandlersToAdd.push_back({ onPress, onRelease });
        }

        void KeyManager::updateKeyHandlers() noexcept
        {
            Lock _(_keyHandlersMutex);

            for (const auto &p : _anyKeyHandlersToAdd)
                _anyKeyHandlers.push_back(std::move(p));
            _anyKeyHandlersToAdd.clear();

            for (const auto &category : _keyHandlersToAdd)
            {
                for (const auto &p : category.second)
                    _keyHandlers[category.first].push_back(std::move(p));
            }
            _keyHandlersToAdd.clear();
        }

        void KeyManager::keyPressed(const OgreBites::KeyboardEvent &event) noexcept
        {
            updateKeyHandlers();

            for (const auto &p : _anyKeyHandlers)
                p.first(event);

            const auto it = _keyHandlers.find(event.keysym.sym);
            if (it != _keyHandlers.end())
                for (const auto &p : it->second)
                    p.first();
        }

        void KeyManager::keyReleased(const OgreBites::KeyboardEvent &event) noexcept
        {
            updateKeyHandlers();

            for (const auto &p : _anyKeyHandlers)
                p.second(event);

            const auto it = _keyHandlers.find(event.keysym.sym);
            if (it != _keyHandlers.end())
                for (const auto &p : it->second)
                    p.second();
        }

        void KeyManager::addMouseMovedHandler(const KeyManager::MouseMovedHandler &func) noexcept
        {
            Lock _(_mouseHandlersMutex);
            _mouseMoveHandlersToAdd.push_back(func);
        }

        void KeyManager::addMouseButtonHandler(const MouseButtonHandler &onPress, const MouseButtonHandler &onRelease) noexcept
        {
            Lock _(_mouseHandlersMutex);
            _mouseButtonHandlersToAdd.push_back({ onPress, onRelease });
        }

        void KeyManager::updateMouseHandlers() noexcept
        {
            Lock _(_mouseHandlersMutex);

            for (const auto &p : _mouseButtonHandlersToAdd)
                _mouseButtonHandlers.push_back(std::move(p));
            _mouseButtonHandlersToAdd.clear();

            for (const auto &p : _mouseMoveHandlersToAdd)
                _mouseMoveHandlers.push_back(std::move(p));
            _mouseMoveHandlersToAdd.clear();
        }

        void KeyManager::mouseMoved(const OgreBites::MouseMotionEvent &event) noexcept
        {
            updateMouseHandlers();
            for (const auto &f : _mouseMoveHandlers)
                f(event);
        }

        void KeyManager::mousePressed(const OgreBites::MouseButtonEvent &event) noexcept
        {
            updateMouseHandlers();
            for (const auto &p : _mouseButtonHandlers)
                p.first(event);
        }

        void KeyManager::mouseReleased(const OgreBites::MouseButtonEvent &event) noexcept
        {
            updateMouseHandlers();
            for (const auto &p : _mouseButtonHandlers)
                p.second(event);
        }
    }
}

