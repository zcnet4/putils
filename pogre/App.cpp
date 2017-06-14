#include "App.hpp"

namespace pogre
{
    bool App::frameRenderingQueued(const Ogre::FrameEvent &event)
    {
        detail::ActionManager::frameRendered(event);
        detail::AnimationManager::frameRendered(event);
        detail::CameraManager::frameRendered(event);
        return OgreBites::ApplicationContext::frameRenderingQueued(event);
    }

    bool App::keyPressed(const OgreBites::KeyboardEvent &event)
    {
        detail::CameraManager::keyPressed(event);
        detail::KeyManager::keyPressed(event);
        return true;
    }

    bool App::keyReleased(const OgreBites::KeyboardEvent &event)
    {
        detail::CameraManager::keyReleased(event);
        detail::KeyManager::keyReleased(event);
        return true;
    }

    bool App::mouseMoved(const OgreBites::MouseMotionEvent &event)
    {
        detail::CameraManager::mouseMoved(event);
        detail::KeyManager::mouseMoved(event);
        return true;
    }

    bool App::mousePressed(const OgreBites::MouseButtonEvent &event)
    {
        detail::CameraManager::mousePressed(event);
        detail::KeyManager::mousePressed(event);
        return true;
    }

    bool App::mouseReleased(const OgreBites::MouseButtonEvent &event)
    {
        detail::CameraManager::mouseReleased(event);
        detail::KeyManager::mouseReleased(event);
        return true;
    }
}
