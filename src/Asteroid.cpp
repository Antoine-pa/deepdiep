#include "World.hpp"
#include "View.hpp"
#include "Asteroid.hpp"

#include <math.h>
#include <iostream>



Asteroid::Asteroid(World* world, float x, float y, float angle, int ttl, int maxHP, int bodyDamage, int rotSpeed) :
    Entity(world, 0, 20.f, View::getShape("square"), sf::Vector2f(x,y), angle, 10) {
    
    ttl_ = ttl;
    maxHP_ = maxHP;
    hp_ = maxHP;
    bodyDamage_ = bodyDamage;

    // Cosmetics : get the square to dance in place :)
    rotSpeed_ = rotSpeed;

}

void Asteroid::update(const GameCmd* command) {
    Entity::update(command);

    if (ttl_>0 && --ttl_==0) // Ignore negative ttl, and kill objects when their ttl reaches 0
        kill();
}
