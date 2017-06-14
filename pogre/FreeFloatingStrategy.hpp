#pragma once

#include "CameraStrategy.hpp"
#include "Point.hpp"

namespace pogre
{
    class FreeFloatingStrategy : public CameraStrategy
    {
    public:
        FreeFloatingStrategy(Ogre::SceneNode *camNode = nullptr, double flightSpeed = 2, double mouseSpeed = 0.1)
                : CameraStrategy(camNode),
                  _flightSpeed(flightSpeed),
                  _mouseSpeed(mouseSpeed)
        {}

    public:
        double getFlightSpeed() const { return _flightSpeed; }
        void setFlightSpeed(double flightSpeed) { _flightSpeed = flightSpeed; }

    public:
        double getMouseSpeed() const { return _mouseSpeed; }
        void setMouseSpeed(double mouseSpeed) { _mouseSpeed = mouseSpeed; }

        /*
         * Implementation
         */

    public:
        void keyPressed(const OgreBites::KeyboardEvent &event) final { applyModifiers(event.keysym.sym, true); }

        void keyReleased(const OgreBites::KeyboardEvent &event) final { applyModifiers(event.keysym.sym, false); }

        void mouseMoved(const OgreBites::MouseMotionEvent &event) final
        {
            auto cam = getCamNode();
            cam->pitch(Ogre::Degree((float)(-event.yrel * _mouseSpeed)), Ogre::Node::TS_LOCAL);
            cam->yaw(Ogre::Degree((float)(-event.xrel * _mouseSpeed)), Ogre::Node::TS_WORLD);
        }

    public:
        void frameRendered(const Ogre::FrameEvent &event) final
        {
            getCamNode()->translate(
                    {
                            (float)(_modifier.x * _flightSpeed),
                            0,
                            (float)(_modifier.y * _flightSpeed)
                    },
                    Ogre::Node::TS_LOCAL
            );
        }

    private:
        void applyModifiers(SDL_Keycode key, bool pressed)
        {
            static const std::unordered_map<SDL_Keycode, putils::Point<int>> _modifiers
                    {
                            { SDLK_UP, { 0, -1 } },
                            { SDLK_w, { 0, -1 } },
                            { SDLK_DOWN, { 0, 1 } },
                            { SDLK_s, { 0, 1 } },
                            { SDLK_LEFT, { -1, 0 } },
                            { SDLK_a, { -1, 0 } },
                            { SDLK_RIGHT, { 1, 0 } },
                            { SDLK_d, { 1, 0 } },
                    };

            const auto it = _modifiers.find(key);
            if (it == _modifiers.end())
                return;

            const auto dir = (pressed ? 1 : -1);
            _modifier.x += it->second.x * dir;
            _modifier.y += it->second.y * dir;
        }

    private:
        double _flightSpeed;
        double _mouseSpeed;
        putils::Point<double> _modifier = { 0, 0 };
    };
}