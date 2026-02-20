#ifndef INCLUDE_GAME_HPP
#define INCLUDE_GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>

class Game {
    std::shared_ptr<sf::RenderWindow> window_;

public:
    Game();
    int run(bool stress = false);
    void quit();
};

#endif
