#include "GameCmd.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>

GameCmd::GameCmd(const sf::Window& relativeTo) {
    up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    fire = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left);
    mousePos_ = sf::Mouse::getPosition(relativeTo);
}

const sf::Vector2i &GameCmd::getMousePos() const {
    return mousePos_;
}
bool GameCmd::pressUp() const {
    return up;
}
bool GameCmd::pressDown() const {
    return down;
}
bool GameCmd::pressLeft() const {
    return left;
}
bool GameCmd::pressRight() const {
    return right;
}
bool GameCmd::pressFire() const {
    return fire;
}
