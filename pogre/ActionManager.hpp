#pragma once

#include <mutex>
#include <vector>
#include <functional>
#include <OGRE/Bites/OgreInput.h>

namespace pogre
{
    namespace detail
    {
        class ActionManager
        {
        public:
            void addAction(const std::function<void(const Ogre::FrameEvent&)>& func) noexcept;
            void addAction(const std::function<void()>& func) noexcept;
            bool actionsDone() noexcept;
            void addPermanentAction(const std::function<void(const Ogre::FrameEvent&)>& func) noexcept;
            void addPermanentAction(const std::function<void()>& func) noexcept;

        protected:
            void frameRendered(const Ogre::FrameEvent& event) noexcept;

        private:
            void updateActions() noexcept;

            std::vector<std::function<void(const Ogre::FrameEvent&)>> _actions;
            std::vector<std::function<void(const Ogre::FrameEvent&)>> _actionsToAdd;
            std::vector<std::function<void(const Ogre::FrameEvent&)>> _permanentActions;
            std::vector<std::function<void(const Ogre::FrameEvent&)>> _permanentActionsToAdd;
            std::mutex _actionsMutex;

            using Lock = std::unique_lock<std::mutex>;
        };
    }
}