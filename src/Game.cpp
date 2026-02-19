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
    // layout.addPlayerToLayout(world->spawnPlayer(2));
    // layout.addPlayerToLayout(world->spawnPlayer(3));
    // layout.addPlayerToLayout(world->spawnPlayer(4));

    // Gestion de l'affichage des FPS :
    sf::Clock fpsClock;
    int frameCount = 0;
    float fps = 0.f;
    bool displayFPS = true;
    sf::Font font;
    if (!font.loadFromFile("Monospace.ttf")) {
        std::cout << "Runtime error: could not load Monospace.ttf font file\n";
        exit(1);
    }
    
    sf::Clock clock;
    while (world->running() && window_->isOpen()) {
        clock.restart(); // Each frame should take exactly 0.03 seconds

        // Take all inputs from the command
        sf::Event event;
        while (window_->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                world->stop();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F5) {
                    displayFPS = !displayFPS;
                    fpsClock.restart();
                    fps = 0.f;
                    frameCount = 0;
                }
            }
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

        
        if (displayFPS) {
            frameCount++;
            if (fpsClock.getElapsedTime().asSeconds() >= 1.f) {
                fps = frameCount / fpsClock.getElapsedTime().asSeconds();
                fpsClock.restart();
                frameCount = 0;
            }
            sf::Text displayedText("FPS : " + (fps <= 1.f ? "--" : std::to_string((int)fps)), font, 18);
            displayedText.setStyle(sf::Text::Bold);
            displayedText.setFillColor(sf::Color::Black);
            displayedText.setPosition(10, 10);
            window_->setView(window_->getDefaultView());
            window_->draw(displayedText);
        }
        window_->display();
        // Sleep until the end of the frame
        struct timespec tim{0, 30000000 - (clock.getElapsedTime().asMicroseconds() * 1000)}, tim2;
        nanosleep(&tim, &tim2);
    }

    // // Done! Display the game outcome until a key is pressed
    // if (window_->isOpen()) {
    //     std::string outcome = world->getStringOutcome();
    //     bool done = false;
    //     while (not done) {
    //         layout.update(outcome, world->getWidth()/2, world->getHeight()/2);
    //         sf::Event event;
    //         while (window_->pollEvent(event))
    //             if (event.type == sf::Event::KeyPressed)
    //                 done = true;
    //         struct timespec tim{0, 30000000 - (clock.getElapsedTime().asMicroseconds() * 1000)}, tim2;
    //         nanosleep(&tim, &tim2);
    //     }
    // }
    return EXIT_SUCCESS;
}

int main(int, char const**) {
    return Game().run();
}
