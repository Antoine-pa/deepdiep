#ifndef INCLUDE_TANK_HPP
#define INCLUDE_TANK_HPP

#include "Entity.hpp"
#include "GameCmd.hpp"
#include "Bullet.hpp"

#include <math.h>
#include <memory>

class World;
class Bullet;

class Tank: public Entity {
    public:
        explicit Tank(World* world);
        ~Tank();

        void fire();
        void update(const GameCmd* command) override;

        int getReloadTime();
        void setReloadTime(int reload);
    
    private:
        int reload_time_ = 5;
        int last_fire_ = 0;
        float impulsionX_ = 0;      // impulsion is the inertia of the shots
        float impulsionY_ = 0;
};

#endif
