#include <algorithm>
#include <OGRE/OgreAnimationState.h>
#include <OGRE/Ogre.h>
#include "AnimationManager.hpp"

namespace pogre
{
    namespace detail
    {
        void AnimationManager::addAnimation(Ogre::AnimationState& anim, bool loop) noexcept
        {
            anim.setLoop(loop);
            anim.setEnabled(true);
            _animations.push_back(&anim);
        }

        void AnimationManager::removeAnimation(Ogre::AnimationState& anim) noexcept
        {
            auto it = std::find(_animations.begin(), _animations.end(), &anim);
            if (it != _animations.end())
            {
                (*it)->setLoop(false);
                (*it)->setEnabled(false);
                _animations.erase(it);
            }
        }

        void AnimationManager::frameRendered(const Ogre::FrameEvent& event) noexcept
        {
            for (const auto anim : _animations)
                anim->addTime(event.timeSinceLastFrame);
        }
    }
}