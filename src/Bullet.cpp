#include "Tank.hpp"
#include "World.hpp"
#include "View.hpp"
#include "Bullet.hpp"

#include <math.h>
#include <iostream>



Bullet::Bullet(World* world, Tank* parent, float x, float y, float angle, float speed, int ttl, float radius) :
    Entity(world, parent->getTeam(), radius, View::getShape("bullet"), sf::Vector2f(x,y), angle, 0), parent_(parent) {
    ttl_ = ttl;
    speed_ = speed;
    maxHP_ = 1;
    hp_ = 1;
    bodyDamage_ = parent->getDamage();
}

void Bullet::update(const GameCmd* command) {
    
    if (ttl_>0 && --ttl_==0) // Ignore negative ttl, and kill objects when their ttl reaches 0
        kill();
    
    sf::Vector2f distance = getDirection() * speed_;
    position_ += distance;

    Entity::update(command);
}

Tank* Bullet::getParent() {
    return parent_;
}
