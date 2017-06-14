#pragma once

#include <mutex>
#include <vector>
#include <OgreFrameListener.h>
#include <OgreAnimationState.h>

namespace pogre
{
    namespace detail
    {
        class AnimationManager
        {
        public:
            void addAnimation(Ogre::AnimationState &anim, bool loop = true) noexcept;
            void removeAnimation(Ogre::AnimationState &anim) noexcept;

        protected:
            void frameRendered(const Ogre::FrameEvent &event) noexcept;

        private:
            std::vector<Ogre::AnimationState *> _animations;
        };
    }
}