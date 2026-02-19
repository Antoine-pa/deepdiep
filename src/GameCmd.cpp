#include "GameCmd.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>

GameCmd::GameCmd(const sf::Window& relativeTo) {
    mousePos_ = sf::Mouse::getPosition(relativeTo);
}

const sf::Vector2i &GameCmd::getMousePos() const {
    return mousePos_;
}
bool GameCmd::pressUp(int i) const {
    return sf::Keyboard::isKeyPressed(presetKeys[i][0]);
}
bool GameCmd::pressDown(int i) const {
    return sf::Keyboard::isKeyPressed(presetKeys[i][1]);
}
bool GameCmd::pressLeft(int i) const {
    return sf::Keyboard::isKeyPressed(presetKeys[i][2]);
}
bool GameCmd::pressRight(int i) const {
    return sf::Keyboard::isKeyPressed(presetKeys[i][3]);
}
bool GameCmd::pressFire(int i) const {
    return sf::Keyboard::isKeyPressed(presetKeys[i][4]);
}
