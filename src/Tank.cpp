#include "Tank.hpp"
#include "World.hpp"
#include "View.hpp"
#include "Bullet.hpp"

#include <math.h>
#include <iostream>


Tank::Tank(World* world, sf::Vector2f pos) :
    Entity(world, 1, 20.f, View::getShape("tank"), pos, 0, 0) {
    position_.x = pos.x;
    position_.y = pos.y;
    setSpeed(5.f);
    maxHP_ = 8;
    hp_ = 8;
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
            move_.y = move_.y < speed_ ? move_.y + inertia_ : speed_;
        if (command->pressUp())
            move_.y = move_.y > -speed_ ? move_.y - inertia_ : -speed_;
    }
    else {
        move_.y = abs(move_.y) - inertia_ > 0 ? (abs(move_.y) - inertia_) * (move_.y / abs(move_.y)) : 0;
    }
    if (command->pressLeft() || command->pressRight()) {
        if (command->pressRight())
            move_.x = move_.x < speed_ ? move_.x + inertia_ : speed_;
        if (command->pressLeft())
            move_.x = move_.x > -speed_ ? move_.x - inertia_ : -speed_;
    }
    else {
        move_.x = abs(move_.x) - inertia_ > 0 ? (abs(move_.x) - inertia_) * (move_.x / abs(move_.x)) : 0;
    }

    // Decrease impulsion after shoot with the time
    if (abs(impulsion_.x) > 0)
        impulsion_.x = abs(impulsion_.x) - inertia_ / 4 > 0 ? (abs(impulsion_.x) - inertia_ / 4) * (impulsion_.x / abs(impulsion_.x)) : 0;
    if (abs(impulsion_.y) > 0)
        impulsion_.y = abs(impulsion_.y) - inertia_ / 4 > 0 ? (abs(impulsion_.y) - inertia_ / 4) * (impulsion_.y / abs(impulsion_.y)) : 0;

    position_.x += move_.x + impulsion_.x;
    position_.y += move_.y + impulsion_.y;
    world_->setCameraPos(position_);    // Update position of camera (centered on player)
    if (command->pressFire())
        fire();
    
    // Mouse relative position after zoom and camera move
    auto relMousePos = sf::Vector2f(command->getMousePos()) - position_ + world_->getCameraPos();
    relMousePos.x -= world_->getWindowWidth() / 2;
    relMousePos.y -= world_->getWindowHeight() / 2;

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
        impulsion_.x -= 2 * inertia_ * cos(getAngle() * 3.14159f / 180.0f);
        impulsion_.y -= 2 * inertia_ * sin(getAngle() * 3.14159f / 180.0f);
        if (abs(impulsion_.x) > speed_ / 2)
            impulsion_.x = speed_  /2 * (impulsion_.x / abs(impulsion_.x));
        if (abs(impulsion_.y) > speed_ / 2)
            impulsion_.y = speed_ / 2 * (impulsion_.y / abs(impulsion_.y));
    }
}

int Tank::getGoalScore() {
    return goalScore;
}
void Tank::setGoalScore(int newGS) {
    goalScore = newGS;
}
