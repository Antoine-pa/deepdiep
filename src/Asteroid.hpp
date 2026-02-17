#ifndef INCLUDE_ASTEROID_HPP
#define INCLUDE_ASTEROID_HPP

#include "Entity.hpp"
#include "GameCmd.hpp"

#include <math.h>
#include <memory>

class World;


class Asteroid: public Entity {
    short ttl_ = -1; // time to live. If negative, infinite

public:
    explicit Asteroid(World* world, float x, float y, sf::Shape* shape, float angle, int ttl, int maxHP, int bodyDamage, int rotSpeed, int xp);

    void update(const GameCmd* command) override;
};


class AsteroidSquare : public Asteroid {
public:
    explicit AsteroidSquare(World* world, float x, float y);
};
class AsteroidTriangle : public Asteroid {
public:
    explicit AsteroidTriangle(World* world, float x, float y);
};
class AsteroidPentagon : public Asteroid {
public:
    explicit AsteroidPentagon(World* world, float x, float y);
};
class AsteroidHexagon : public Asteroid {
public:
    explicit AsteroidHexagon(World* world, float x, float y);
};


#endif
