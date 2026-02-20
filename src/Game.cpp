#include "Game.hpp"
#include "World.hpp"
#include "Layout.hpp"
#include "GameCmd.hpp"
#include "Menu.hpp"
#include <iostream>


static const int width = 800;
static const int heigth = 600;

Game::Game() : window_(std::make_shared<sf::RenderWindow>(sf::VideoMode(width, heigth), "tiep", sf::Style::Titlebar | sf::Style::Close)) {
    window_->setKeyRepeatEnabled(false);
}

int Game::run(bool stress) {
    world = std::make_shared<World>(2000, 2000, width, heigth, stress);
    layout = std::make_shared<Layout>(world, window_);


    // Gestion de l'affichage des FPS :
    sf::Clock fpsClock;
    int frameCount = 0;
    float fps = 0.f;
    bool displayFPS = false;
    sf::Font font;
    if (!font.loadFromFile("Monospace.ttf")) {
        std::cout << "Runtime error: could not load Monospace.ttf font file\n";
        exit(1);
    }
    bool inMenu = true;

    Menu* currentMenu;
    Menu mainMenu(font, 0.f, 0.f, world->getWindowWidth(), world->getWindowHeight(), 60.f);
    Menu optionMainMenu(font, 0.f, 0.f, world->getWindowWidth(), world->getWindowHeight(), 60.f);
    Menu optionKeySetMenu(font, 0.f, 0.f, world->getWindowWidth(), world->getWindowHeight(), 60.f);
    Menu optionKeyBindMenu(font, 0.f, 0.f, world->getWindowWidth(), world->getWindowHeight(), 60.f);
    Menu endGame(font, 0.f, 0.f, world->getWindowWidth(), world->getWindowHeight(), 60.f);

    // Main Menu :
    mainMenu.addItem("DeepDiep", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, nullptr);
    mainMenu.addItem("Start", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [this, &inMenu, stress](){ startGame(stress); inMenu=false; });
    mainMenu.addItem("Options", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [&optionMainMenu, &currentMenu](){ currentMenu = &optionMainMenu; });
    mainMenu.addItem("Quit", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [this](){ world->stop(); running = false; });

    // Option Menu :
    optionMainMenu.addItem("Options", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, nullptr);
    optionMainMenu.addItem("Add Player", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [this](){
        if (layout->players_.size() < 4) layout->addPlayerToLayout(world->spawnPlayer(layout->players_.size()+1));
    });
    optionMainMenu.addItem("Remove Player", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [this](){
        if (layout->players_.size() > 1) layout->removePlayerToLayout(layout->players_[layout->players_.size()-1]);
    });
    optionMainMenu.addItem("KeyBind options", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [&optionKeySetMenu, &currentMenu](){ currentMenu = &optionKeySetMenu; });
    optionMainMenu.addItem("Back to main menu", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [&mainMenu, &currentMenu](){ currentMenu = &mainMenu; });
    
    // Option KeySet Menu :
    int player_keybind = 0;
    optionKeySetMenu.addItem("Choose the player for KeySet", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, nullptr);
    optionKeySetMenu.addItem("Player1", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){ player_keybind = 0; optionKeyBindMenu.updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset()); currentMenu = &optionKeyBindMenu; });
    optionKeySetMenu.addItem("Player2", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){ player_keybind = 1 % layout->players_.size(); optionKeyBindMenu.updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset()); currentMenu = &optionKeyBindMenu; });
    optionKeySetMenu.addItem("Player3", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){ player_keybind = 2 % layout->players_.size(); optionKeyBindMenu.updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset()); currentMenu = &optionKeyBindMenu; });
    optionKeySetMenu.addItem("Player4", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){ player_keybind = 3 % layout->players_.size(); optionKeyBindMenu.updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset()); currentMenu = &optionKeyBindMenu; });
    optionKeySetMenu.addItem("Back", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [&optionMainMenu, &currentMenu](){ currentMenu = &optionMainMenu; });

    // Option KeyBind Menu :
    optionKeyBindMenu.addItem("Choose the key to bind", 32, world->getWindowWidth() / 2, 20, true, nullptr);
    optionKeyBindMenu.addItem("Move mode :", 20, 50, 20, false, [this, &player_keybind, &optionKeyBindMenu]()
        { layout->players_[player_keybind]->getKeyset()->move_type = (KeySet::Move)((layout->players_[player_keybind]->getKeyset()->move_type + 1) % 2);
          optionKeyBindMenu.updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset()); });
    optionKeyBindMenu.addItem("Move Up :", 20, 50, 20, false,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){
            currentMenu = nullptr;
            sf::Event event;
            while (true) {
                while (window_->pollEvent(event)) {
                    if (event.type == sf::Event::KeyPressed) {
                        layout->players_[player_keybind]->getKeyset()->up[0] = event.key.code;
                        currentMenu = &optionKeyBindMenu;
                        currentMenu->updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset());
                        return;
                    }
                }
            }
        });
    optionKeyBindMenu.addItem("Move Down :", 20, 50, 20, false,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){
            currentMenu = nullptr;
            sf::Event event;
            while (true) {
                while (window_->pollEvent(event)) {
                    if (event.type == sf::Event::KeyPressed) {
                        layout->players_[player_keybind]->getKeyset()->down[0] = event.key.code;
                        currentMenu = &optionKeyBindMenu;
                        currentMenu->updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset());
                        return;
                    }
                }
            }
        });
    optionKeyBindMenu.addItem("Move Left :", 20, 50, 20, false,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){
            currentMenu = nullptr;
            sf::Event event;
            while (true) {
                while (window_->pollEvent(event)) {
                    if (event.type == sf::Event::KeyPressed) {
                        layout->players_[player_keybind]->getKeyset()->left[0] = event.key.code;
                        currentMenu = &optionKeyBindMenu;
                        currentMenu->updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset());
                        return;
                    }
                }
            }
        });
    optionKeyBindMenu.addItem("Move Right :", 20, 50, 20, false,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){
            currentMenu = nullptr;
            sf::Event event;
            while (true) {
                while (window_->pollEvent(event)) {
                    if (event.type == sf::Event::KeyPressed) {
                        layout->players_[player_keybind]->getKeyset()->right[0] = event.key.code;
                        currentMenu = &optionKeyBindMenu;
                        currentMenu->updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset());
                        return;
                    }
                }
            }
        });
    optionKeyBindMenu.addItem("Aim mode :", 20, world->getWindowWidth() * 2 / 3, -280, false, [this, &player_keybind, &optionKeyBindMenu]()
        { layout->players_[player_keybind]->getKeyset()->aim_type = (KeySet::Aim)((layout->players_[player_keybind]->getKeyset()->aim_type + 1) % 3);
          optionKeyBindMenu.updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset()); });
    optionKeyBindMenu.addItem("View Up :", 20, world->getWindowWidth() * 2 / 3, -280, false,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){
            currentMenu = nullptr;
            sf::Event event;
            while (true) {
                while (window_->pollEvent(event)) {
                    if (event.type == sf::Event::KeyPressed) {
                        layout->players_[player_keybind]->getKeyset()->vup[0] = event.key.code;
                        currentMenu = &optionKeyBindMenu;
                        currentMenu->updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset());
                        return;
                    }
                }
            }
        });
    optionKeyBindMenu.addItem("View Down :", 20, world->getWindowWidth() * 2 / 3, -280, false,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){
            currentMenu = nullptr;
            sf::Event event;
            while (true) {
                while (window_->pollEvent(event)) {
                    if (event.type == sf::Event::KeyPressed) {
                        layout->players_[player_keybind]->getKeyset()->vdown[0] = event.key.code;
                        currentMenu = &optionKeyBindMenu;
                        currentMenu->updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset());
                        return;
                    }
                }
            }
        });
    optionKeyBindMenu.addItem("View Left :", 20, world->getWindowWidth() * 2 / 3, -280, false,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){
            currentMenu = nullptr;
            sf::Event event;
            while (true) {
                while (window_->pollEvent(event)) {
                    if (event.type == sf::Event::KeyPressed) {
                        layout->players_[player_keybind]->getKeyset()->vleft[0] = event.key.code;
                        currentMenu = &optionKeyBindMenu;
                        currentMenu->updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset());
                        return;
                    }
                }
            }
        });
    optionKeyBindMenu.addItem("View Right :", 20, world->getWindowWidth() * 2 / 3, -280, false,
        [this, &optionKeyBindMenu, &currentMenu, &player_keybind](){
            currentMenu = nullptr;
            sf::Event event;
            while (true) {
                while (window_->pollEvent(event)) {
                    if (event.type == sf::Event::KeyPressed) {
                        layout->players_[player_keybind]->getKeyset()->vright[0] = event.key.code;
                        currentMenu = &optionKeyBindMenu;
                        currentMenu->updateMenuFromKeySet(layout->players_[player_keybind]->getKeyset());
                        return;
                    }
                }
            }
        });
    optionKeyBindMenu.addItem("Back", 32, world->getWindowWidth() / 2, -280, true, [&optionKeySetMenu, &currentMenu](){ currentMenu = &optionKeySetMenu; });
    
    // End Game :
    endGame.addItem("End of Game", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, nullptr);
    endGame.addItem("Replay", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [this, &inMenu, stress](){ startGame(stress); inMenu=false; });
    endGame.addItem("Options", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [&optionMainMenu, &currentMenu](){ currentMenu = &optionMainMenu; });
    endGame.addItem("Quit", 32, world->getWindowWidth() / 2, world->getWindowHeight() / 3, true, [this](){ world->stop(); running = false;});


    currentMenu = &mainMenu;

    sf::Clock clock;

    while (running) {

        while (world->running() && window_->isOpen()) {
            clock.restart(); // Each frame should take exactly 0.03 seconds

            // Take all inputs from the command
            sf::Event event;
            while (window_->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    world->stop();
                    running = false;
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::F5) {
                        displayFPS = !displayFPS;
                        fpsClock.restart();
                        fps = 0.f;
                        frameCount = 0;
                    }
                    else if (inMenu && event.key.code == sf::Keyboard::Down) {
                        currentMenu->addSelectedIndex(true);
                    }
                    else if (inMenu && event.key.code == sf::Keyboard::Up) {
                        currentMenu->addSelectedIndex(false);
                    }
                    else if (inMenu && event.key.code == sf::Keyboard::Enter) {
                        currentMenu->execute();
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                world->stop();
            }
            GameCmd cmd(*window_); // This object captures the state of the commands: mouse position + up/down/left/right/fire buttons' states

            // Update the world state by one step
            world->update(cmd);

            // Display the world state, but only if we did not run out of time for this frame
            if (clock.getElapsedTime() < sf::milliseconds(30000))
                layout->update();
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

            if (inMenu) {
                currentMenu->update(*window_);
                currentMenu->draw(*window_);
            }

            window_->display();
            // Sleep until the end of the frame
            struct timespec tim{0, 30000000 - (clock.getElapsedTime().asMicroseconds() * 1000)}, tim2;
            nanosleep(&tim, &tim2);
        }
        inMenu = true;
        currentMenu = &endGame;
        startGame(stress);
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

void Game::startGame(bool stress) {
    std::shared_ptr<World> newWorld = std::make_shared<World>(world->getWidth(), world->getHeight(), world->getWindowWidth(), world->getWindowHeight(), stress);
    std::shared_ptr<Layout> newLayout = std::make_shared<Layout>(newWorld, window_);
    bool first = true;
    for (auto &player : layout->players_) {
        if (first) {
            newWorld->tanks[0]->setKeyset(player->getKeyset());
            first = false;
            continue;
        }
        auto t = newWorld->spawnPlayer(player->getTeam());
        t->setKeyset(player->getKeyset());
        newLayout->addPlayerToLayout(t);
    }
    world = newWorld;
    layout = newLayout;
}

int main(int argv, char const** argc) {
    if (argv > 1) {
        std::string m = argc[1];
        if (m == "stressTest")
            return Game().run(true);
    }
    return Game().run();
}

