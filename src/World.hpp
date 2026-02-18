#ifndef INCLUDE_WORLD_HPP
#define INCLUDE_WORLD_HPP

#include "Entity.hpp"
#include "Tank.hpp"
#include "Bullet.hpp"
#include "Asteroid.hpp"

class View;

class World {
    friend View;

    bool running_ = true;
    unsigned long tick_ = 0;
    int width_;
    int height_;

    int windowWidth_;
    int windowHeight_;
    sf::Vector2f cameraPos_;
    float cameraZoom_;

public:
    World(int width, int height, int windowWidth, int windowHeight);
    int getWidth() { return width_;}
    int getHeight() { return height_;}
    unsigned long getTick() {return tick_;}

    void update(const GameCmd& cmd);

    bool running() { return running_;}
    void stop() {running_ = false;}
    std::string getStringOutcome();
    void push(EntityPtr e);

    sf::Vector2f getCameraPos(sf::Vector2f tankPos) const;
    float getCameraZoom() const;
    void setCameraZoom(float zoom);
    int getWindowWidth() const;
    int getWindowHeight() const;

private:
    std::vector<std::shared_ptr<Tank>> tanks; // Active entities, that must be passed a GameControl when updating
    std::vector<std::shared_ptr<Entity>> entities; // All entities (active and passive)
};

#endif
