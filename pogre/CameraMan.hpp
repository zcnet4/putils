#pragma once

#include <memory>

#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>

#include "CameraStrategy.hpp"
#include "FreeFloatingStrategy.hpp"

namespace pogre
{
    class CameraMan
    {
    public:
        CameraMan(Ogre::SceneManager &scnMgr, Ogre::RenderWindow &window,
                  std::unique_ptr<CameraStrategy> &&strategy = std::make_unique<FreeFloatingStrategy>(),
                  int ZOrder = 0, float left = 0, float top = 0, float width = 1, float height = 1
        )
                : _camera(scnMgr.createCamera("CameraMan")),
                  _camNode(scnMgr.getRootSceneNode()->createChildSceneNode()),
                  _strategy(std::move(strategy))
        {
            _camera->setNearClipDistance(5);
            _camera->setAutoAspectRatio(true);
            window.addViewport(_camera, ZOrder, left, top, width, height);

            _camNode->attachObject(_camera);

            if (_strategy)
                _strategy->setCamNode(_camNode);
        }

        CameraMan() = default;

        CameraMan(CameraMan &&) = default;
        CameraMan &operator=(CameraMan &&) = default;

    public:
        void disable() noexcept { _enabled = false; }
        void enable() noexcept { _enabled = true; }
        bool isEnabled() const noexcept { return _enabled; }
        void setEnabled(bool enabled) noexcept { _enabled = enabled; }

    public:
        Ogre::Camera &getCamera() { return *_camera; }
        const Ogre::Camera &getCamera() const { return *_camera; }

    public:
        Ogre::SceneNode &getNode() { return *_camNode; }
        const Ogre::SceneNode &getNode() const { return *_camNode; }

    public:
        template<typename T>
        T &getStrategy() { return static_cast<T&>(*_strategy); }

        template<typename T>
        const T &getStrategy() const { return static_cast<const T &>(*_strategy); }

        void setStrategy(std::unique_ptr<CameraStrategy> &&strategy)
        {
            _strategy = std::move(strategy);
            _strategy->setCamNode(_camNode);
        }

    public:
        void keyPressed(const OgreBites::KeyboardEvent &event) { if (_enabled) _strategy->keyPressed(event); }
        void keyReleased(const OgreBites::KeyboardEvent &event) { if (_enabled) _strategy->keyReleased(event); }

    public:
        void mouseMoved(const OgreBites::MouseMotionEvent &event) { if (_enabled) _strategy->mouseMoved(event); }
        void mousePressed(const OgreBites::MouseButtonEvent &event) { if (_enabled) _strategy->mousePressed(event); }
        void mouseReleased(const OgreBites::MouseButtonEvent &event) { if (_enabled) _strategy->mouseReleased(event); }

    public:
        void frameRendered(const Ogre::FrameEvent &event) { if (_enabled) _strategy->frameRendered(event); }

    private:
        bool _enabled = true;
        Ogre::Camera *_camera;
        Ogre::SceneNode *_camNode;
        std::unique_ptr<CameraStrategy> _strategy;
    };
}
