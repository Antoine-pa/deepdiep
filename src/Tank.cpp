#include "Tank.hpp"
#include "World.hpp"
#include "View.hpp"
#include "Bullet.hpp"

#include <math.h>
#include <iostream>


Tank::Tank(World* world, sf::Vector2f pos, int team) :
    Entity(world, team, 20.f, View::getShape("tank"), pos, 0, 0) {
    position_.x = pos.x;
    position_.y = pos.y;
    setSpeed(5.f);
    maxHP_ = 8;
    hp_ = 8;
    empty_shape_ = View::getShape("empty tank");

    ks_ = new KeySet();
}
Tank::~Tank() = default;

int Tank::getReloadTime() { return reload_time_; }
void Tank::setReloadTime(int reload) { reload_time_ = reload; }

void Tank::update(const GameCmd* command) {

    if (command == nullptr) // Call two times each tick, one with this argument
        return;
    
    sf::Vector2f move = command->getMove(ks_);
    move_ += move * inertia_;
    if (move_.x*move_.x + move_.y*move_.y > speed_*speed_) {
        float spd = sqrtf(move_.x*move_.x + move_.y*move_.y);
        move_.x *= speed_ / spd;
        move_.y *= speed_ / spd;
    }
    if (ks_->aim_type == KeySet::Aim::AKeys) {
        ks_->keyMousePos_ += (sf::Vector2i)(command->getMove(ks_, true) * inertia_ * 4.f);
        if (ks_->keyMousePos_.x*ks_->keyMousePos_.x + ks_->keyMousePos_.y*ks_->keyMousePos_.y > radius_ * radius_ * 4) {
            float spd = sqrtf(ks_->keyMousePos_.x*ks_->keyMousePos_.x + ks_->keyMousePos_.y*ks_->keyMousePos_.y);
            ks_->keyMousePos_.x *= radius_ * 4 / spd;
            ks_->keyMousePos_.y *= radius_ * 4 / spd;
        }
    }

    // if we don't move we slow down
    if (move.x == 0)
        move_.x = abs(move_.x) - inertia_ > 0 ? (abs(move_.x) - inertia_) * (move_.x / abs(move_.x)) : 0;
    if (move.y == 0)
        move_.y = abs(move_.y) - inertia_ > 0 ? (abs(move_.y) - inertia_) * (move_.y / abs(move_.y)) : 0;

    // Decrease recoil impulsion after shoot with the time
    if (abs(impulsion_.x) > 0)
        impulsion_.x = abs(impulsion_.x) - inertia_ / 6 > 0 ? (abs(impulsion_.x) - inertia_ / 6) * (impulsion_.x / abs(impulsion_.x)) : 0;
    if (abs(impulsion_.y) > 0)
        impulsion_.y = abs(impulsion_.y) - inertia_ / 6 > 0 ? (abs(impulsion_.y) - inertia_ / 6) * (impulsion_.y / abs(impulsion_.y)) : 0;

    position_.x += move_.x + impulsion_.x;
    position_.y += move_.y + impulsion_.y;
    if (command->pressFire(ks_))
        fire();
    
    
    auto angle = 0;
    
    if (ks_->aim_type == KeySet::Aim::AMouse) {
        auto relMousePos = getRelativeMousePos(command->getMousePos(ks_));
        angle = atan2(relMousePos.y,relMousePos.x)*180./3.14159;
    }
    else {
        // Mouse relative position after zoom and camera move
        auto relMousePos = command->getMousePos(ks_);
        angle = atan2(relMousePos.y,relMousePos.x)*180./3.14159;
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

float Tank::getZoom() {
    return zoom_;
}

sf::Shape* Tank::getEmptyShape() {
    return empty_shape_;
}
void Tank::setEmptyShape(sf::Shape* shape) {
    empty_shape_ = shape;
}
sf::View Tank::getViewport() {
    return viewport_;
}
void Tank::setViewport(sf::View viewport) {
    viewport_ = viewport;
}
void Tank::setViewportSize(sf::Vector2f worldSize) {
    viewport_.setSize(sf::Vector2f(worldSize.x * viewport_.getViewport().width, worldSize.y * viewport_.getViewport().height));
}
sf::Vector2f Tank::getRelativeMousePos(sf::Vector2i mousePos) {
    sf::Vector2f result = sf::Vector2f(mousePos.x, mousePos.y);
    result -= sf::Vector2f(world_->getWindowWidth() * viewport_.getViewport().left, world_->getWindowHeight() * viewport_.getViewport().top);
    result -= sf::Vector2f(world_->getWindowWidth() * viewport_.getViewport().width / 2, world_->getWindowHeight() * viewport_.getViewport().height / 2);
    result += world_->getCameraPos(this) - position_;
    return result;
}
KeySet* Tank::getKeyset() const {
    return ks_;
}




Destroyer::Destroyer(World* world, sf::Vector2f pos, int team) :
    Tank(world, pos, team) {
        damage_ *= 2;
        reload_time_ *= 2;
        setShape(View::getShape("destroyer"));
        setEmptyShape(View::getShape("empty destroyer"));
    }

Destroyer::~Destroyer() = default;
