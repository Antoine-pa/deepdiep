#include "Tank.hpp"
#include "World.hpp"
#include "View.hpp"
#include "Bullet.hpp"

#include <math.h>
#include <iostream>



Bullet::Bullet(World* world, Tank* parent, float x, float y, float angle, float speed, int ttl, int bodyDamage) :
    Entity(world, parent->getTeam(), 2.f, View::getShape("bullet"), sf::Vector2f(x,y), angle, 0), parent_(parent) {
    ttl_ = ttl;
    speed_ = speed;
    maxHP_ = 1;
    hp_ = 1;
    bodyDamage_ = bodyDamage;
}

void Bullet::update(const GameCmd* command) {
    
    (void)(command);

    if (ttl_>0 && --ttl_==0) // Ignore negative ttl, and kill objects when their ttl reaches 0
        kill();
    
    sf::Vector2f distance = getDirection() * speed_;
    position_ += distance;
    if (position_.x < -radius_ || position_.x > world_->getWidth() || position_.y < -radius_ || position_.y > world_->getHeight()) {
        kill();
    }

}
