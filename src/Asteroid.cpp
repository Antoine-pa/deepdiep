#include "World.hpp"
#include "View.hpp"
#include "Asteroid.hpp"

#include <math.h>
#include <iostream>
#include <random>
#include <functional>


Asteroid::Asteroid(World* world, float x, float y, sf::Shape* shape, float angle, int ttl, int maxHP, int bodyDamage, int rotSpeed, int xp) :
    Entity(world, 0, 20.f, shape, sf::Vector2f(x,y), angle, 10) {
    
    ttl_ = ttl;
    maxHP_ = maxHP;
    hp_ = maxHP;
    bodyDamage_ = bodyDamage;
    xp_ = xp;

    // Cosmetics : get the square to dance in place :)
    rotSpeed_ = rotSpeed;

}

void Asteroid::update(const GameCmd* command) {
    Entity::update(command);

    if (ttl_>0 && --ttl_==0) // Ignore negative ttl, and kill objects when their ttl reaches 0
        kill();
}


std::random_device rnd;
AsteroidSquare::AsteroidSquare(World* world, float x, float y) :
    Asteroid(world, x, y, View::getShape("triangle"), 0, -1, 10 /*HP*/, 8 /*Damage*/, 0, 10 /*XP*/) {

    auto squareRotationalSpeedGenerator = std::bind(std::uniform_int_distribution<>(-4, 3),  std::mt19937(rnd()));
    int rotSpeed = squareRotationalSpeedGenerator();
    rotSpeed += (rotSpeed < 0 ? -3:4);
    setRotSpeed(rotSpeed);
}
AsteroidTriangle::AsteroidTriangle(World* world, float x, float y) :
    Asteroid(world, x, y, View::getShape("triangle"), 0, -1, 30 /*HP*/, 8 /*Damage*/, 0, 25 /*XP*/) {

    auto squareRotationalSpeedGenerator = std::bind(std::uniform_int_distribution<>(-3, 2),  std::mt19937(rnd()));
    int rotSpeed = squareRotationalSpeedGenerator();
    rotSpeed += (rotSpeed < 0 ? 0:1);
    setRotSpeed(rotSpeed);
}
AsteroidPentagon::AsteroidPentagon(World* world, float x, float y) :
    Asteroid(world, x, y, View::getShape("pentagon"), 0, -1, 100 /*HP*/, 12 /*Damage*/, 0, 130 /*XP*/) {

    auto squareRotationalSpeedGenerator = std::bind(std::uniform_int_distribution<>(-1, 0),  std::mt19937(rnd()));
    int rotSpeed = squareRotationalSpeedGenerator();
    rotSpeed += (rotSpeed < 0 ? 0:1);
    setRotSpeed(rotSpeed);
}
AsteroidHexagon::AsteroidHexagon(World* world, float x, float y) :
    Asteroid(world, x, y, View::getShape("hexagon"), 0, -1, 1500 /*HP*/, 20 /*Damage*/, 0, 1500 /*XP*/) {

    auto squareRotationalSpeedGenerator = std::bind(std::uniform_int_distribution<>(-1, 0),  std::mt19937(rnd()));
    int rotSpeed = squareRotationalSpeedGenerator();
    rotSpeed += (rotSpeed < 0 ? 0:1);
    setRotSpeed(rotSpeed);
}
