#include "GameCmd.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <math.h>

GameCmd::GameCmd(const sf::Window& relativeTo) {
    mousePos_ = sf::Mouse::getPosition(relativeTo);
}

sf::Vector2i GameCmd::getMousePos(KeySet* ks) const {
    if (ks->aim_type == KeySet::Aim::AMouse)
        return mousePos_;
    else if (ks->aim_type == KeySet::Aim::AJoystick)
        return sf::Vector2i(
            sf::Joystick::getAxisPosition(ks->joystick, ks->haim_joy),
            sf::Joystick::getAxisPosition(ks->joystick, ks->vaim_joy)
        );
    return ks->keyMousePos_;
}

bool pressUp(KeySet* ks, bool view = false) {
    if (view) return (sf::Keyboard::isKeyPressed(ks->vup[0]) || sf::Keyboard::isKeyPressed(ks->vup[1]));
    return (sf::Keyboard::isKeyPressed(ks->up[0]) || sf::Keyboard::isKeyPressed(ks->up[1]));
}
bool pressDown(KeySet* ks, bool view = false) {
    if (view) return (sf::Keyboard::isKeyPressed(ks->vdown[0]) || sf::Keyboard::isKeyPressed(ks->vdown[1]));
    return (sf::Keyboard::isKeyPressed(ks->down[0]) || sf::Keyboard::isKeyPressed(ks->down[1]));
}
bool pressLeft(KeySet* ks, bool view = false) {
    if (view) return (sf::Keyboard::isKeyPressed(ks->vleft[0]) || sf::Keyboard::isKeyPressed(ks->vleft[1]));
    return (sf::Keyboard::isKeyPressed(ks->left[0]) || sf::Keyboard::isKeyPressed(ks->left[1]));
}
bool pressRight(KeySet* ks, bool view = false) {
    if (view) return (sf::Keyboard::isKeyPressed(ks->vright[0]) || sf::Keyboard::isKeyPressed(ks->vright[1]));
    return (sf::Keyboard::isKeyPressed(ks->right[0]) || sf::Keyboard::isKeyPressed(ks->right[1]));
}
sf::Vector2f GameCmd::getMove(KeySet* ks, bool view) const {
    if (view) {
        return sf::Vector2f(pressRight(ks, view) - pressLeft(ks, view), pressDown(ks, view) - pressUp(ks, view));
    }
    if (ks->move_type == KeySet::Move::MKeys) {
        sf::Vector2f move = sf::Vector2f(pressRight(ks) - pressLeft(ks), pressDown(ks) - pressUp(ks));
        if (abs(move.x) + abs(move.y) == 2) return move / sqrtf(2);
        return move;
    }
    else {
        return sf::Vector2f(sf::Joystick::getAxisPosition(ks->joystick, ks->hmove_joy), sf::Joystick::getAxisPosition(ks->joystick, ks->vmove_joy)) / 100.f;
    }
}
bool GameCmd::pressFire(KeySet* ks) const {
    if (ks->move_type == KeySet::Move::MKeys)
        return sf::Keyboard::isKeyPressed(ks->fire[0]) || sf::Keyboard::isKeyPressed(ks->fire[1]);
    else {
        return sf::Joystick::isButtonPressed(ks->joystick, ks->fire_joy[0]) || sf::Joystick::isButtonPressed(ks->joystick, ks->fire_joy[1]);
    }
}
