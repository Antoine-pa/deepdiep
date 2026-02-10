#include "GameCmd.hpp"
#include "Tank.hpp"
#include "World.hpp"
#include "View.hpp"

#include <math.h>
#include <iostream>

Tank::Tank(World* world) :
    Entity(world, Entity::Team::PLAYER, 20.f, View::getShape("tank"), sf::Vector2f(0.f, 0.f), 0) {
    position_.x = radius_ * 2;
    position_.y = radius_ * 2;
    setSpeed(5.f);
    maxHP_ = 6;
    hp_ = 6;
    bodyDamage_ = 2;
}
Tank::~Tank() = default;

int Tank::getReloadTime() { return reload_time_; }
void Tank::setReloadTime(int reload) { reload_time_ = reload; }

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

    if (position_.x - radius_ < 0.f)
        position_.x = radius_;
    else if (position_.x + radius_ > world_->getWidth())
        position_.x = world_->getWidth() - radius_;

    if (position_.y - radius_ < 0.f)
        position_.y = radius_;
    else if (position_.y + radius_ > world_->getHeight())
        position_.y = world_->getHeight() - radius_;
}

void Tank::fire() {
    if ((int)(world_->getTick()) - last_fire_ >= reload_time_) {
        world_->push(Entity::makeBullet(world_, getTeam(), getPosition().x, getPosition().y, getAngle()));
        last_fire_ = world_->getTick();
    }
}
