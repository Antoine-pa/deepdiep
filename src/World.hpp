#ifndef INCLUDE_WORLD_HPP
#define INCLUDE_WORLD_HPP

#include "Entity.hpp"
#include "Tank.hpp"

class View;

class World {
    friend View;

    bool running_ = true;
    unsigned long tick_ = 0;
    int width_;
    int height_;
public:
    World(int width, int height);
    int getWidth() { return width_;}
    int getHeight() { return height_;}
    unsigned long getTick() {return tick_;}

    void update(const GameCmd& cmd);

    bool running() { return running_;}
    void stop() {running_ = false;}
    std::string getStringOutcome();
    void push(EntityPtr e);

private:
    std::vector<std::shared_ptr<Tank>> tanks; // Active entities, that must be passed a GameControl when updating
    std::vector<std::shared_ptr<Entity>> entities; // All entities (active and passive)
};

#endif
