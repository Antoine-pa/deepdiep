#include "World.hpp"
#include <iostream>
#include <random>
#include <functional>


World::World(int width, int height, int windowWidth, int windowHeight, bool stress):
    width_(width), height_(height), windowWidth_(windowWidth), windowHeight_(windowHeight) {
    
    std::random_device rd;
    // Bind together uniform distributions on the right intervals with random engines that are seeded with the random_device
    auto randX     = std::bind(std::uniform_int_distribution<>(0, width_-1),  std::mt19937(rd()));
    auto randY     = std::bind(std::uniform_int_distribution<>(0, height_-1), std::mt19937(rd()));

    auto tank = std::make_shared<Tank>(this, sf::Vector2f(randX(), randY()), 1);
    
    tanks.push_back(tank);
    entities.push_back(tank);

    for (int i=0; i < (stress ? width * height / 1000 : width * height / 20000); i++) {
        int pX = randX(), pY = randY();
        if ((pX-tank->getPosition().x)*(pX-tank->getPosition().x) + (pY-tank->getPosition().y)*(pY-tank->getPosition().y) < tank->getRadius() * tank->getRadius() * 16) {
            i--;
        }
        else {
            int asteroidType = std::bind(std::uniform_int_distribution<>(0, 100),  std::mt19937(rd()))();
            if (asteroidType < 50) {
                entities.push_back(std::shared_ptr<Entity>(new AsteroidSquare(this, pX, pY)));
            } else if (asteroidType < 80) {
                entities.push_back(std::shared_ptr<Entity>(new AsteroidTriangle(this, pX, pY)));
            } else if (asteroidType < 98) {
                entities.push_back(std::shared_ptr<Entity>(new AsteroidPentagon(this, pX, pY)));
            } else {
                entities.push_back(std::shared_ptr<Entity>(new AsteroidHexagon(this, pX, pY)));
            }
        }
    }
    if (stress) {
        auto randTeam = std::bind(std::uniform_int_distribution<>(1, 15), std::mt19937(rd()));
        for (int i = 0; i < width * height / 20000; i++) {
            spawnPlayer(randTeam());
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

sf::Vector2f World::getCameraPos(Tank* tank) const {
    sf::Vector2f tankPos = tank->getPosition();
    float zoom = tank->getZoom();

    // Il faut encore vérifier que la caméra s'arête au bord de l'écran si on dépasse (en prenant en compte le zoom)
    float halfViewW = (windowWidth_ * tank->getViewport().getViewport().width / 2.) / zoom;
    float halfViewH = (windowHeight_ * tank->getViewport().getViewport().height / 2.) / zoom;
    if (tankPos.x + halfViewW > width_) {
        tankPos.x = width_ - halfViewW;
    }
    else if (tankPos.x - halfViewW < 0) {
        tankPos.x = halfViewW;
    }
    if (tankPos.y + halfViewH > height_) {
        tankPos.y = height_ - halfViewH;
    }
    else if (tankPos.y - halfViewH < 0) {
        tankPos.y = halfViewH;
    }
    return tankPos;
}

std::shared_ptr<Tank> World::spawnPlayer(int team) {
    std::random_device rd;
    auto randX     = std::bind(std::uniform_int_distribution<>(0, width_-1),  std::mt19937(rd()));
    auto randY     = std::bind(std::uniform_int_distribution<>(0, height_-1), std::mt19937(rd()));

    auto tank = std::make_shared<Tank>(this, sf::Vector2f(randX(), randY()), team);

    // The distance with the nearest ennemy need to be high
    for (auto &t : tanks) {
        if (t->getTeam() != team) {
            if ((t->getPosition().x-tank->getPosition().x)*(t->getPosition().x-tank->getPosition().x) + (t->getPosition().y-tank->getPosition().y)*(t->getPosition().y-tank->getPosition().y) < tank->getRadius() * tank->getRadius() * 64) {
                return spawnPlayer(team);
            }
        }
    }
    for (auto &e : entities) {
        if ((e->getPosition().x-tank->getPosition().x)*(e->getPosition().x-tank->getPosition().x) + (e->getPosition().y-tank->getPosition().y)*(e->getPosition().y-tank->getPosition().y) < tank->getRadius() * tank->getRadius() * 16) {
            e->kill();
        }
    }
    tanks.push_back(tank);
    entities.push_back(tank);
    return std::shared_ptr<Tank>(tank);
}

int World::getWindowWidth() const {
    return windowWidth_;
}
int World::getWindowHeight() const {
    return windowHeight_;
}
