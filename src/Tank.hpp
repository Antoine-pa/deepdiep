#ifndef INCLUDE_TANK_HPP
#define INCLUDE_TANK_HPP

#include "Entity.hpp"
#include "GameCmd.hpp"

#include <math.h>
#include <memory>

class World;

class Tank: public Entity {
    public:
        explicit Tank(World* world);
        ~Tank();

        void fire();
        void update(const GameCmd* command) override;
};
#endif
