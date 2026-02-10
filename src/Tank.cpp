#include "GameCmd.hpp"
#include "Tank.hpp"
#include "World.hpp"
#include "View.hpp"

#include <math.h>
#include <iostream>

Tank::Tank(World* world) :
    Entity(world, Entity::Team::PLAYER, 20., View::getShape("tank"), sf::Vector2f(world->getWidth() / 2, world->getHeight() / 2), 0) {
    setSpeed(3.f);
    maxHP_ = 6;
    hp_ = 6;
    bodyDamage_ = 2;
}
Tank::~Tank() = default;

void Tank::update(const GameCmd* command) {
    if (command == nullptr)
        return;

    if (command->pressUp())
        position_.y -= speed_;
    if (command->pressDown())
        position_.y += speed_;
    if (command->pressRight())
        position_.x += speed_;
    if (command->pressLeft())
        position_.x -= speed_;
    if (command->pressFire())
        fire();

    auto relMousePos = sf::Vector2f(command->getMousePos()) - position_;

    auto angle = 0;
    if (relMousePos.x == 0) {
        if (relMousePos.y>0)
            angle= 90;
        else
            angle = -90;
    } else {
        angle = atan2(relMousePos.y,relMousePos.x)*180./3.14156;
        // See https://www.dsprelated.com/showarticle/1052.php if atan2 becomes an issue (what I doubt)
    }
    setAngle(angle);

    if (position_.x < -10.0f)
        position_.x = world_->getWidth();
    else if (position_.x > world_->getWidth())
        position_.x = 0.0f;

    if (position_.y < -10.0f)
        position_.y = world_->getHeight();
    else if (position_.y > world_->getHeight())
        position_.y = 0.0f;
}

void Tank::fire() {
    world_->push(Entity::makeBullet(world_, getTeam(), getPosition().x, getPosition().y, getAngle()));
}
