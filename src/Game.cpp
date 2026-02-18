#include "Game.hpp"
#include "World.hpp"
#include "Layout.hpp"
#include "GameCmd.hpp"
#include <iostream>

static const int width = 800;
static const int heigth = 600;

Game::Game() : window_(std::make_shared<sf::RenderWindow>(sf::VideoMode(width, heigth), "tiep", sf::Style::Titlebar | sf::Style::Close)) {
    window_->setKeyRepeatEnabled(false);
}

int Game::run() {
    auto world = std::make_shared<World>(2000, 2000, width, heigth);
    Layout layout(world, window_);
    layout.addPlayerToLayout(world->spawnPlayer(2));
    layout.addPlayerToLayout(world->spawnPlayer(3));
    layout.addPlayerToLayout(world->spawnPlayer(4));
    sf::Clock clock;
    
    while (world->running() && window_->isOpen()) {

        clock.restart(); // Each frame should take exactly 0.03 seconds

        // Take all inputs from the command
        sf::Event event;
        while (window_->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                world->stop();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            world->stop();
        GameCmd cmd(*window_); // This object captures the state of the commands: mouse position + up/down/left/right/fire buttons' states

        // Update the world state by one step
        world->update(cmd);

        // Display the world state, but only if we did not run out of time for this frame
        if (clock.getElapsedTime() < sf::milliseconds(30000))
            layout.update();
        else
            std::cout << "Skipping frame rendering!\n";

        // Sleep until the end of the frame
        struct timespec tim{0, 30000000 - (clock.getElapsedTime().asMicroseconds() * 1000)}, tim2;
        nanosleep(&tim, &tim2);
    }

    // Done! Display the game outcome until a key is pressed
    if (window_->isOpen()) {
        std::string outcome = world->getStringOutcome();
        bool done = false;
        while (not done) {
            layout.update(outcome, world->getWidth()/2, world->getHeight()/2);
            sf::Event event;
            while (window_->pollEvent(event))
                if (event.type == sf::Event::KeyPressed)
                    done = true;
            struct timespec tim{0, 30000000 - (clock.getElapsedTime().asMicroseconds() * 1000)}, tim2;
            nanosleep(&tim, &tim2);
        }
    }
    return EXIT_SUCCESS;
}

int main(int, char const**) {
    return Game().run();
}
