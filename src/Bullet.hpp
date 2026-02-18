#ifndef INCLUDE_BULLET_HPP
#define INCLUDE_BULLET_HPP

#include "Entity.hpp"
#include "GameCmd.hpp"
#include "Tank.hpp"

#include <math.h>
#include <memory>

class World;
class Tank;


class Bullet: public Entity {
    short ttl_ = -1; // time to live. If negative, infinite
    Tank* parent_;

public:
    explicit Bullet(World* world, Tank* parent, float x, float y, float angle, float speed, int ttl);

    void update(const GameCmd* command) override;
    Tank* getParent();
};



#endif
