#ifndef INCLUDE_GAMECTRL_HPP
#define INCLUDE_GAMECTRL_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include "array"


class KeySet {

public:
    enum Move { MKeys, MJoystick };
    enum Aim { AMouse, AKeys, AJoystick };
    
    int joystick = 0;
    enum Move move_type = MKeys;
    enum Aim aim_type = AMouse;

    // Move keybind
    std::vector<sf::Keyboard::Key> up = {sf::Keyboard::Up, sf::Keyboard::Unknown};
    std::vector<sf::Keyboard::Key> down = {sf::Keyboard::Down, sf::Keyboard::Unknown};
    std::vector<sf::Keyboard::Key> left = {sf::Keyboard::Left, sf::Keyboard::Unknown};
    std::vector<sf::Keyboard::Key> right = {sf::Keyboard::Right, sf::Keyboard::Unknown};
    std::vector<sf::Keyboard::Key> fire = {sf::Keyboard::Space, sf::Keyboard::Unknown};
    std::vector<int> fire_joy = {0, 1};

    // View keybind
    sf::Joystick::Axis hmove_joy = sf::Joystick::Axis::X;
    sf::Joystick::Axis vmove_joy = sf::Joystick::Axis::Y;
    sf::Joystick::Axis haim_joy = sf::Joystick::Axis::R;
    sf::Joystick::Axis vaim_joy = sf::Joystick::Axis::U;

    std::vector<sf::Keyboard::Key> vup = {sf::Keyboard::Z, sf::Keyboard::Unknown};
    std::vector<sf::Keyboard::Key> vdown = {sf::Keyboard::S, sf::Keyboard::Unknown};
    std::vector<sf::Keyboard::Key> vleft = {sf::Keyboard::Q, sf::Keyboard::Unknown};
    std::vector<sf::Keyboard::Key> vright = {sf::Keyboard::D, sf::Keyboard::Unknown};
    sf::Vector2i keyMousePos_ = sf::Vector2i(0, 0);
};


class GameCmd {
    sf::Vector2i mousePos_;

public:
    GameCmd(const sf::Window& relativeTo);

    sf::Vector2i getMousePos(KeySet* ks) const;
    sf::Vector2f getMove(KeySet* ks, bool view = false) const;
    bool pressFire(KeySet* ks) const;
};

#endif
