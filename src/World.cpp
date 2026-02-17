#include "World.hpp"
#include <iostream>
#include <random>
#include <functional>


World::World(int width, int height): width_(width), height_(height) {
    std::random_device rd;
    auto tank = std::make_shared<Tank>(this);   // Rajouter la position d'apparition ?
    
    // Bind together uniform distributions on the right intervals with random engines that are seeded with the random_device
    auto randX     = std::bind(std::uniform_int_distribution<>(0, width_-1),  std::mt19937(rd()));
    auto randY     = std::bind(std::uniform_int_distribution<>(0, height_-1), std::mt19937(rd()));
    auto randAngle = std::bind(std::uniform_int_distribution<>(0, 359),       std::mt19937(rd()));
    
    tanks.push_back(tank);
    entities.push_back(tank);


    auto squareRotationalSpeedGenerator = std::bind(std::uniform_int_distribution<>(-4, 3),  std::mt19937(rd()));
    for (int i=0; i < 30; i++) {
        int pX = randX(), pY = randY();
        if ((pX-tank.get()->getPosition().x)*(pX-tank.get()->getPosition().x) + (pY-tank.get()->getPosition().y)*(pY-tank.get()->getPosition().y) < tank.get()->getRadius() * tank.get()->getRadius() * 16) {
            i--;
        }
        else {
            int rotSpeed = squareRotationalSpeedGenerator();
            rotSpeed += (rotSpeed < 0 ? -3:4);
            entities.push_back(std::shared_ptr<Entity>(new Asteroid(this, pX, pY, randAngle(), -1, 3, 3, rotSpeed)));
        }
    }
}

void World::update(const GameCmd& cmd) {
    tick_ ++;

    // Update all game entities
    for (auto &tank: tanks)
        tank->update(&cmd);
    for (auto &e: entities)
        e->update();

    // Check for collisions
    for (auto &a : entities) {
        for (auto &b : entities) {
            if (a->isAlive() && b->isAlive() && (a->getTeam() != b->getTeam()) && a->collides(b)) { // HIT
                a->hitBy(b);
                b->hitBy(a);
            }
        }
    }

    // remove dead entities from the array -- the order does not matters
    for (unsigned i=0; i<entities.size();i++) {
        if (not entities[i]->isAlive()) {
            entities[i--] = entities.back();
            entities.pop_back();
        }
    }
    for (unsigned i=0; i<tanks.size();i++) {
        if (not tanks[i]->isAlive()) {
            tanks[i--] = tanks.back();
            tanks.pop_back();
        }
    }
    if (entities.size() == tanks.size() || tanks.empty())
        stop();
}
std::string World::getStringOutcome() {
    if (tanks.empty())
        return std::string("You loose");
    if (entities.size() == tanks.size())
        return std::string("You win");
    return std::string("(game still ongoing)");
}

void World::push(EntityPtr e) {
    entities.push_back(e);
}

