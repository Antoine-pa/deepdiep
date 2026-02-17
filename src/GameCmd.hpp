#ifndef INCLUDE_GAMECTRL_HPP
#define INCLUDE_GAMECTRL_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>

class GameCmd {
    sf::Vector2i mousePos_;
    bool up, down, left, right, fire, a, e;
public:
    GameCmd(const sf::Window& relativeTo);

    const sf::Vector2i& getMousePos() const;
    bool pressUp() const;
    bool pressDown() const;
    bool pressLeft() const;
    bool pressRight() const;
    bool pressFire() const;
    bool pressA() const;
    bool pressE() const;
};

#endif
