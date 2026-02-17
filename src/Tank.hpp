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

        int getGoalScore();
        void setGoalScore(int newGS);
    
    private:
        int reload_time_ = 5;
        int last_fire_ = 0;
        sf::Vector2f impulsion_ = sf::Vector2f(0.f, 0.f);      // impulsion is the inertia of the shots

        int goalScore = 100;
};

#endif
