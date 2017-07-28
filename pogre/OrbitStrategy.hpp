#pragma once

#include "CameraStrategy.hpp"

namespace pogre
{
    class OrbitStrategy : public CameraStrategy
    {
    public:
        OrbitStrategy(Ogre::SceneNode *camNode = nullptr, Ogre::SceneNode *target = nullptr,
                      bool mouseEnabled = false, bool fixedYaw = false, bool fixedPitch = false,
                      const Ogre::Vector3 &distance = { 0, 0, 10 }, bool inheritOrientation = true,
                      double speed = 0.1)
                : CameraStrategy(camNode),
                  _target(target), _speed(speed), _distance(distance),
                  _inheritOrientation(inheritOrientation),
                  _mouseEnabled(mouseEnabled),
                  _fixedYaw(fixedYaw), _fixedPitch(fixedPitch)
        {
            if (target && camNode)
                setTarget(target);
        }

    public:
        Ogre::SceneNode *getTarget() const { return _target; }
        void setTarget(Ogre::SceneNode *target)
        {
            const auto cam = getCamNode();

            cam->getParentSceneNode()->removeChild(cam);
            if (_fixedRotation)
                target->removeChild(_fixedRotation);

            _fixedRotation = target->createChildSceneNode();
            _fixedRotation->setInheritOrientation(_inheritOrientation);
            _fixedRotation->addChild(cam);

            // target->addChild(cam);
            cam->setPosition(Ogre::Vector3::ZERO);
            cam->translate(-_distance);
            cam->yaw(Ogre::Degree(180));

            _target = target;
        }

    public:
        bool isMouseEnabled() const noexcept { return _mouseEnabled; }
        void setMouseEnabled(bool mouseEnabled) noexcept { _mouseEnabled = mouseEnabled; }

    public:
        bool isInheritOrientation() const noexcept { return _inheritOrientation; }
        void setInheritOrientation(bool inheritOrientation) noexcept
        {
            _inheritOrientation = inheritOrientation;
            _fixedRotation->setInheritOrientation(inheritOrientation);
        }

    public:
        bool isFixedYaw() const noexcept { return _fixedYaw; }
        void setFixedYaw(bool fixedYaw) noexcept { _fixedYaw = fixedYaw; }

        bool isFixedPitch() const noexcept { return _fixedPitch; }
        void setFixedPitch(bool fixedPitch) noexcept { _fixedPitch = fixedPitch; }

    public:
        double getSpeed() const noexcept { return _speed; }
        void setSpeed(double speed) noexcept { _speed = speed; }

    public:
        const Ogre::Vector3 &getDistance() const noexcept { return _distance; }
        void setDistance(const Ogre::Vector3 &distance) noexcept
        {
            _distance = distance;
            if (_target)
                setTarget(_target);
        }

    public:
        void resetOrientation()
        {
            auto cam = getCamNode();
            cam->resetOrientation();
            cam->yaw(Ogre::Degree(180));
        }

        /*
         * Implementation
         */

    public:
        void mousePressed(const OgreBites::MouseButtonEvent &event) override
        {
            _mouseDown = true;
        }

        void mouseReleased(const OgreBites::MouseButtonEvent &event) override
        {
            _mouseDown = false;
        }

        void mouseMoved(const OgreBites::MouseMotionEvent &event)
        {
            if (!_target || (_mouseEnabled && !_mouseDown))
                return;

            auto cam = getCamNode();
            // const auto dist = (cam->getPosition() - Ogre::Vector3::ZERO).length();

            // const auto pos = cam->getPosition();
            cam->setPosition(Ogre::Vector3::ZERO);

            if (!_fixedPitch)
                cam->pitch(Ogre::Degree(-event.yrel * _speed), Ogre::Node::TS_LOCAL);
            if (!_fixedYaw)
                cam->yaw(Ogre::Degree(-event.xrel * _speed), Ogre::Node::TS_WORLD);

            cam->translate(_distance, Ogre::Node::TS_LOCAL);
        }

    private:
        Ogre::SceneNode *_fixedRotation = nullptr;
        Ogre::SceneNode *_target;

    private:
        Ogre::Vector3 _distance;
        bool _inheritOrientation;
        double _speed;

    private:
        bool _mouseEnabled;
        bool _mouseDown;

    private:
        bool _fixedYaw;
        bool _fixedPitch;
    };
}