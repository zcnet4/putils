#pragma once

#include <SDL.h>
#include <CEGUI/CEGUI.h>
#include "Point.hpp"

namespace pogre
{
    template<typename Scancode>
    CEGUI::Key::Scan SDL_ScanCodeToCEGUIKey(Scancode key)
    {
        switch (key)
        {
            case SDL_SCANCODE_ESCAPE: return CEGUI::Key::Escape;
            case SDL_SCANCODE_F1: return CEGUI::Key::F1;
            case SDL_SCANCODE_F2: return CEGUI::Key::F2;
            case SDL_SCANCODE_F3: return CEGUI::Key::F3;
            case SDL_SCANCODE_F4: return CEGUI::Key::F4;
            case SDL_SCANCODE_F5: return CEGUI::Key::F5;
            case SDL_SCANCODE_F6: return CEGUI::Key::F6;
            case SDL_SCANCODE_F7: return CEGUI::Key::F7;
            case SDL_SCANCODE_F8: return CEGUI::Key::F8;
            case SDL_SCANCODE_F9: return CEGUI::Key::F9;
            case SDL_SCANCODE_F10: return CEGUI::Key::F10;
            case SDL_SCANCODE_F11: return CEGUI::Key::F11;
            case SDL_SCANCODE_F12: return CEGUI::Key::F12;
            case SDL_SCANCODE_F13: return CEGUI::Key::F13;
            case SDL_SCANCODE_F14: return CEGUI::Key::F14;
            case SDL_SCANCODE_F15: return CEGUI::Key::F15;
            case SDL_SCANCODE_UP: return CEGUI::Key::ArrowUp;
            case SDL_SCANCODE_DOWN: return CEGUI::Key::ArrowDown;
            case SDL_SCANCODE_LEFT: return CEGUI::Key::ArrowLeft;
            case SDL_SCANCODE_RIGHT: return CEGUI::Key::ArrowRight;
            case SDL_SCANCODE_LSHIFT: return CEGUI::Key::LeftShift;
            case SDL_SCANCODE_RSHIFT: return CEGUI::Key::RightShift;
            case SDL_SCANCODE_LCTRL: return CEGUI::Key::LeftControl;
            case SDL_SCANCODE_RCTRL: return CEGUI::Key::RightControl;
            case SDL_SCANCODE_LALT: return CEGUI::Key::LeftAlt;
            case SDL_SCANCODE_RALT: return CEGUI::Key::RightAlt;
            case SDL_SCANCODE_TAB: return CEGUI::Key::Tab;
            case SDL_SCANCODE_RETURN: return CEGUI::Key::Return;
            case SDL_SCANCODE_BACKSPACE: return CEGUI::Key::Backspace;
            case SDL_SCANCODE_INSERT: return CEGUI::Key::Insert;
            case SDL_SCANCODE_DELETE: return CEGUI::Key::Delete;
            case SDL_SCANCODE_PAGEUP: return CEGUI::Key::PageUp;
            case SDL_SCANCODE_PAGEDOWN: return CEGUI::Key::PageDown;
            case SDL_SCANCODE_HOME: return CEGUI::Key::Home;
            case SDL_SCANCODE_END: return CEGUI::Key::End;
            case SDL_SCANCODE_KP_ENTER: return CEGUI::Key::NumpadEnter;
            case SDL_SCANCODE_SPACE: return CEGUI::Key::Space;
            case SDL_SCANCODE_A: return CEGUI::Key::A;
            case SDL_SCANCODE_B: return CEGUI::Key::B;
            case SDL_SCANCODE_C: return CEGUI::Key::C;
            case SDL_SCANCODE_D: return CEGUI::Key::D;
            case SDL_SCANCODE_E: return CEGUI::Key::E;
            case SDL_SCANCODE_F: return CEGUI::Key::F;
            case SDL_SCANCODE_G: return CEGUI::Key::G;
            case SDL_SCANCODE_H: return CEGUI::Key::H;
            case SDL_SCANCODE_I: return CEGUI::Key::I;
            case SDL_SCANCODE_J: return CEGUI::Key::J;
            case SDL_SCANCODE_K: return CEGUI::Key::K;
            case SDL_SCANCODE_L: return CEGUI::Key::L;
            case SDL_SCANCODE_M: return CEGUI::Key::M;
            case SDL_SCANCODE_N: return CEGUI::Key::N;
            case SDL_SCANCODE_O: return CEGUI::Key::O;
            case SDL_SCANCODE_P: return CEGUI::Key::P;
            case SDL_SCANCODE_Q: return CEGUI::Key::Q;
            case SDL_SCANCODE_R: return CEGUI::Key::R;
            case SDL_SCANCODE_S: return CEGUI::Key::S;
            case SDL_SCANCODE_T: return CEGUI::Key::T;
            case SDL_SCANCODE_U: return CEGUI::Key::U;
            case SDL_SCANCODE_V: return CEGUI::Key::V;
            case SDL_SCANCODE_W: return CEGUI::Key::W;
            case SDL_SCANCODE_X: return CEGUI::Key::X;
            case SDL_SCANCODE_Y: return CEGUI::Key::Y;
            case SDL_SCANCODE_Z: return CEGUI::Key::Z;
            case SDL_SCANCODE_1: return CEGUI::Key::One;
            case SDL_SCANCODE_2: return CEGUI::Key::Two;
            case SDL_SCANCODE_3: return CEGUI::Key::Three;
            case SDL_SCANCODE_4: return CEGUI::Key::Four;
            case SDL_SCANCODE_5: return CEGUI::Key::Five;
            case SDL_SCANCODE_6: return CEGUI::Key::Six;
            case SDL_SCANCODE_7: return CEGUI::Key::Seven;
            case SDL_SCANCODE_8: return CEGUI::Key::Eight;
            case SDL_SCANCODE_9: return CEGUI::Key::Nine;
            case SDL_SCANCODE_0: return CEGUI::Key::Zero;
            default: return CEGUI::Key::Unknown;
        }
    }

    template<typename SDLButton>
    inline CEGUI::MouseButton SDL_toMouseButton(SDLButton button)
    {
        switch (button)
        {
            case SDL_BUTTON_LEFT: return CEGUI::LeftButton;
            case SDL_BUTTON_RIGHT: return CEGUI::RightButton;
            case SDL_BUTTON_MIDDLE: return CEGUI::MiddleButton;
            default: return CEGUI::LeftButton;
        }
    }

    template<typename Precision = double>
    inline void setWidgetRelativeSize(CEGUI::Window &win, const putils::Point<Precision> &size)
    {
        win.setSize({{ (float)size.x, 0 }, { (float)size.y, 0 }});
    }

    template<typename Precision = double>
    void setWidgetCenter(CEGUI::Window &win, const putils::Point<Precision> &pos)
    {
        const auto size = win.getSize();
        win.setPosition(
                {
                        { (float)(pos.x - size.d_width.d_scale / 2), 0 },
                        { (float)(pos.y - size.d_height.d_scale / 2), 0 }
                }
        );
    }

    inline std::vector<Ogre::RaySceneQueryResultEntry> getSelectedEntities(CEGUI::MouseCursor &mouse, Ogre::SceneManager &scnMgr, Ogre::RenderWindow &win, Ogre::Camera &cam)
    {
        auto mousePos = mouse.getPosition();
        Ogre::Ray mouseRay = cam.getCameraToViewportRay(
                mousePos.d_x / win.getWidth(), mousePos.d_y / win.getHeight()
        );

        auto query = scnMgr.createRayQuery(Ogre::Ray());
        query->setRay(mouseRay);
        query->setSortByDistance(true);

        return query->execute();
    }
}
