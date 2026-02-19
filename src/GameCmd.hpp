#ifndef INCLUDE_GAMECTRL_HPP
#define INCLUDE_GAMECTRL_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "array"

class GameCmd {
    sf::Vector2i mousePos_;
    
    std::array<std::array<sf::Keyboard::Key, 5>, 3> presetKeys 
    {{
        { sf::Keyboard::Z, sf::Keyboard::S, sf::Keyboard::Q, sf::Keyboard::D, sf::Keyboard::LShift },
        { sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::RShift },
        { sf::Keyboard::O, sf::Keyboard::L, sf::Keyboard::K, sf::Keyboard::M, sf::Keyboard::Space },
    }};

public:
    GameCmd(const sf::Window& relativeTo);

    const sf::Vector2i& getMousePos() const;
    bool pressUp(int i) const;
    bool pressDown(int i) const;
    bool pressLeft(int i) const;
    bool pressRight(int i) const;
    bool pressFire(int i) const;
};

#endif
