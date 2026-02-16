#include "Entity.hpp"
#include "World.hpp"
#include "View.hpp"

#include <math.h>
#include <unordered_map>
#include <random>
#include <functional>

Entity::Entity(World* world, int team, float radius, sf::Shape* shape, sf::Vector2f pos, float angle, int xp) :
    shape_(shape), position_(pos), radius_(radius), team_(team), world_(world)
{
    setAngle(angle);
    setXp(xp);
}

std::random_device rd;
std::function<int(void)> squareRotationalSpeedGenerator = std::bind(std::uniform_int_distribution<>(-4, 3),  std::mt19937(rd()));

EntityPtr Entity::makeSquare(World* world, float x, float y, float angle) {
    auto e = std::make_shared<Entity>(world, 0 /*=ASTEROID*/, 20.f, View::getShape("square"), sf::Vector2f(x,y), angle, 10);

    e->maxHP_ = 3;
    e->hp_ = 3;
    e->bodyDamage_ = 3;

    // Cosmetics : get the square to dance in place :)
    e->rotSpeed_ = squareRotationalSpeedGenerator(); // First get something in [-4;3]
    e->rotSpeed_ += (e->rotSpeed_<0 ? -3:4); // Change it into something in [-8;-4] or [4;8] but not in between

    return e;
}
// EntityPtr Entity::makeBullet(World* world, int team, float x, float y, float angle) {
//     auto b = std::make_shared<Entity>(world, team, 2.f, View::getShape("bullet"), sf::Vector2f(x,y), angle, 0);
//     b->speed_ = 27.f;

//     b->maxHP_ = 1;
//     b->hp_ = 1;
//     b->bodyDamage_ = 1;
//     return b;
// }

void Entity::update(const GameCmd*) {
    if (!alive_)
        return;

    if (rotSpeed_ != 0)
        setAngle(getAngle()+rotSpeed_);

    sf::Vector2f distance = direction_ * speed_;
    position_ += distance;
    if (position_.x < -radius_ || position_.x > world_->getWidth() || position_.y < -radius_ || position_.y > world_->getHeight()) {
        kill();
    }
}

bool Entity::collides(const EntityPtr& other) {
    float ax = position_.x;
    float ay = position_.y;

    float px = other->position_.x;
    float py = other->position_.y;

    float sqrDistance = ((ax - px) * (ax - px)) + ((ay - py) * (ay - py));
    float sqrRadius = (radius_ + other->radius_) * (radius_ + other->radius_);

    return (sqrDistance <= sqrRadius);
}
void Entity::hitBy(const EntityPtr& other) {
    hp_ -= other->bodyDamage_;
    if (hp_<=0) {
        kill();
    }
}
/* --------- Getters/Setters --------------- */
float Entity::getAngle() const {
    return angle_;
}
void Entity::setAngle(float angle) {
    angle_ = angle;
    direction_.x = cos(angle * 3.14159f / 180.0f);
    direction_.y = sin(angle * 3.14159f / 180.0f);
}
const sf::Vector2f& Entity::getDirection() const {
    return direction_;
}

float Entity::getRotSpeed() const {
    return  rotSpeed_;
}
void  Entity::setRotSpeed(float rs) {
    rotSpeed_ = rs;
}
float Entity::getSpeed() const {
    return  speed_;
}
void  Entity::setSpeed(float s) {
    speed_ = s;
}
float Entity::getXp() const {
    return xp_;
}
void  Entity::setXp(float xp) {
    xp_ = xp;
}

float Entity::getRadius() const {
    return radius_;
}
const sf::Vector2f& Entity::getPosition() const {
    return position_;
}
int Entity::getTeam() const {
    return team_;
}
bool Entity::isAlive() const {
    return alive_;
}
float Entity::getHPRatio() const {
    return 1.*hp_/maxHP_;
}

sf::Shape* Entity::getShape() const {
    return shape_;
}
void Entity::kill() {
    alive_ = false;
}

/* --------- Operators --------------- */

bool Entity::operator==(const Entity& other) const {
    return typeid(*this) == typeid(other) &&  position_ == other.position_ && angle_ == other.angle_ && rotSpeed_ == other.rotSpeed_ && speed_ == other.speed_;
}
bool Entity::operator!=(const Entity& other) const {
    return not (*this == other);
}



