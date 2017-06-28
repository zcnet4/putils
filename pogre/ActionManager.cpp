#include "ActionManager.hpp"

namespace pogre
{
    namespace detail
    {
        void ActionManager::addAction(const std::function<void(const Ogre::FrameEvent &)> &func) noexcept
        {
            Lock _(_actionsMutex);
            _actionsToAdd.push_back(func);
        }

        void ActionManager::addAction(const std::function<void()> &func) noexcept
        {
            Lock _(_actionsMutex);
            _actionsToAdd.push_back([func](auto &&) { func(); });
        }

        bool ActionManager::actionsDone() noexcept
        {
            Lock _(_actionsMutex);
            return _actions.size() == 0;
        }

        void ActionManager::addPermanentAction(const std::function<void(const Ogre::FrameEvent &)> &func) noexcept
        {
            Lock _(_actionsMutex);
            _permanentActionsToAdd.push_back(func);
        }

        void ActionManager::addPermanentAction(const std::function<void()> &func) noexcept
        {
            Lock _(_actionsMutex);
            _permanentActionsToAdd.push_back([func](auto &&){ func(); });
        }

        void ActionManager::updateActions() noexcept
        {
            Lock _(_actionsMutex);

            for (const auto &f : _permanentActionsToAdd)
                _permanentActions.push_back(std::move(f));
            _permanentActionsToAdd.clear();

            for (const auto &f : _actionsToAdd)
                _actions.push_back(std::move(f));
            _actionsToAdd.clear();
        }

        void ActionManager::frameRendered(const Ogre::FrameEvent &event) noexcept
        {
            updateActions();
            for (const auto &f : _permanentActions)
                f(event);

            for (const auto &f : _actions)
                f(event);
            _actions.clear();
        }
    }
}

