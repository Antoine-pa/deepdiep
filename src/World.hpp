#ifndef INCLUDE_WORLD_HPP
#define INCLUDE_WORLD_HPP

#include "Entity.hpp"
#include "Tank.hpp"
#include "Bullet.hpp"
#include "Asteroid.hpp"

class View;
class Layout;

class World {
    friend View;
    friend Layout;

    bool running_ = true;
    unsigned long tick_ = 0;
    int width_;
    int height_;

    int windowWidth_;
    int windowHeight_;

public:
    World(int width, int height, int windowWidth, int windowHeight, bool stress = false);
    int getWidth() { return width_;}
    int getHeight() { return height_;}
    unsigned long getTick() {return tick_;}

    void update(const GameCmd& cmd);

    bool running() { return running_;}
    void stop() {running_ = false;}
    std::string getStringOutcome();
    void push(EntityPtr e);

    std::shared_ptr<Tank> spawnPlayer(int team);

    sf::Vector2f getCameraPos(Tank* tank) const;

    void startGame(); // Create a new world with the same parameters that the current world
    
    int getWindowWidth() const;
    int getWindowHeight() const;

private:
    std::vector<std::shared_ptr<Tank>> tanks; // Active entities, that must be passed a GameControl when updating
    std::vector<std::shared_ptr<Entity>> entities; // All entities (active and passive)
};

#endif
