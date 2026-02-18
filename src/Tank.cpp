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
    empty_shape_ = View::getShape("empty tank");
}
Tank::~Tank() = default;

int Tank::getReloadTime() { return reload_time_; }
void Tank::setReloadTime(int reload) { reload_time_ = reload; }

void Tank::update(const GameCmd* command) {

    if (command == nullptr) // Call two times each tick, one with this argument
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
        impulsion_.x = abs(impulsion_.x) - inertia_ / 6 > 0 ? (abs(impulsion_.x) - inertia_ / 6) * (impulsion_.x / abs(impulsion_.x)) : 0;
    if (abs(impulsion_.y) > 0)
        impulsion_.y = abs(impulsion_.y) - inertia_ / 6 > 0 ? (abs(impulsion_.y) - inertia_ / 6) * (impulsion_.y / abs(impulsion_.y)) : 0;

    position_.x += move_.x + impulsion_.x;
    position_.y += move_.y + impulsion_.y;
    if (command->pressFire())
        fire();
    
    // Mouse relative position after zoom and camera move
    auto relMousePos = sf::Vector2f(command->getMousePos()) - position_ + world_->getCameraPos(position_);
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
            new Bullet(world_, this, getPosition().x, getPosition().y, getAngle(), 27.f, 25)));
        last_fire_ = world_->getTick();
        
        // Add recoil of weapons 
        impulsion_.x -= inertia_ * cos(getAngle() * 3.14159f / 180.0f);
        impulsion_.y -= inertia_ * sin(getAngle() * 3.14159f / 180.0f);
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

int Tank::getDamage() {
    return damage_;
}
void Tank::setDamage(int newDamage) {
    damage_ = newDamage;
}

sf::Shape* Tank::getEmptyShape() {
    return empty_shape_;
}
void Tank::setEmptyShape(sf::Shape* shape) {
    empty_shape_ = shape;
}

Destroyer::Destroyer(World* world, sf::Vector2f pos) :
    Tank(world, pos) {
        damage_ *= 2;
        reload_time_ *= 2;
        setShape(View::getShape("destroyer"));
        setEmptyShape(View::getShape("empty destroyer"));
    }

Destroyer::~Destroyer() = default;