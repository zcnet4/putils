#pragma once

#include <OgreInput.h>
#include <OgreSceneNode.h>

namespace pogre
{
    class CameraStrategy
    {
    public:
        CameraStrategy(Ogre::SceneNode *camNode = nullptr) : _camNode(camNode) {}
        virtual ~CameraStrategy() = default;

    public:
        virtual void keyPressed(const OgreBites::KeyboardEvent &event) {}
        virtual void keyReleased(const OgreBites::KeyboardEvent &event) {}

    public:
        virtual void mouseMoved(const OgreBites::MouseMotionEvent &event) {}
        virtual void mousePressed(const OgreBites::MouseButtonEvent &event) {}
        virtual void mouseReleased(const OgreBites::MouseButtonEvent &event) {}

    public:
        virtual void frameRendered(const Ogre::FrameEvent &event) {}

    public:
        void setCamNode(Ogre::SceneNode *node) { _camNode = node; }
        Ogre::SceneNode *getCamNode() const { return _camNode; }

    private:
        Ogre::SceneNode *_camNode;
    };
}