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
    explicit Asteroid(World* world, float x, float y, float angle, int ttl, int maxHP, int bodyDamage, int rotSpeed);

    void update(const GameCmd* command) override;
};



#endif
