#include "Tank.hpp"
#include "World.hpp"
#include "View.hpp"
#include "Bullet.hpp"

#include <math.h>
#include <iostream>


Tank::Tank(World* world) :
    Entity(world, 1, 20.f, View::getShape("tank"), sf::Vector2f(0.f, 0.f), 0, 10) {
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
    
    if (command->pressUp() || command->pressDown()) {
        if (command->pressDown())
            moveY_ = moveY_ < speed_ ? moveY_ + inertia_ : speed_;
        if (command->pressUp())
            moveY_ = moveY_ > -speed_ ? moveY_ - inertia_ : -speed_;
    }
    else {
        moveY_ = abs(moveY_) - inertia_ > 0 ? (abs(moveY_) - inertia_) * (moveY_ / abs(moveY_)) : 0;
    }
    if (command->pressLeft() || command->pressRight()) {
        if (command->pressRight())
            moveX_ = moveX_ < speed_ ? moveX_ + inertia_ : speed_;
        if (command->pressLeft())
            moveX_ = moveX_ > -speed_ ? moveX_ - inertia_ : -speed_;
    }
    else {
        moveX_ = abs(moveX_) - inertia_ > 0 ? (abs(moveX_) - inertia_) * (moveX_ / abs(moveX_)) : 0;
    }

    // Decrease impulsion after shoot with the time
    if (abs(impulsionX_) > 0)
        impulsionX_ = abs(impulsionX_) - inertia_ / 4 > 0 ? (abs(impulsionX_) - inertia_ / 4) * (impulsionX_ / abs(impulsionX_)) : 0;
    if (abs(impulsionY_) > 0)
        impulsionY_ = abs(impulsionY_) - inertia_ / 4 > 0 ? (abs(impulsionY_) - inertia_ / 4) * (impulsionY_ / abs(impulsionY_)) : 0;

    position_.x += moveX_ + impulsionX_;
    position_.y += moveY_ + impulsionY_;
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
        angle = atan2(relMousePos.y,relMousePos.x)*180./3.14159;
        // See https://www.dsprelated.com/showarticle/1052.php if atan2 becomes an issue (what I doubt)
    }
    setAngle(angle);

    // The tank can't exit the board
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
        world_->push(std::shared_ptr<Entity>(
            new Bullet(world_, this, getPosition().x, getPosition().y, getAngle(), 27.f, 25, 1)));
        last_fire_ = world_->getTick();
        
        // Add recoil of weapons 
        impulsionX_ -= 2 * inertia_ * cos(getAngle() * 3.14159f / 180.0f);
        impulsionY_ -= 2 * inertia_ * sin(getAngle() * 3.14159f / 180.0f);
        if (abs(impulsionX_) > speed_ / 2)
            impulsionX_ = speed_  /2 * (impulsionX_ / abs(impulsionX_));
        if (abs(impulsionY_) > speed_ / 2)
            impulsionY_ = speed_ / 2 * (impulsionY_ / abs(impulsionY_));
    }
}

