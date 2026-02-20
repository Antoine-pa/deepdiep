#ifndef INCLUDE_GAME_HPP
#define INCLUDE_GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "View.hpp"

class Game {
    std::shared_ptr<sf::RenderWindow> window_;
    std::shared_ptr<World> world;
    std::shared_ptr<Layout> layout;
    bool running = true;

public:
    Game();
    int run(bool stress = false);
    void quit();

    void startGame(bool stress = false);
};



#endif
