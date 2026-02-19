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
    xp_ = xp;
}

std::random_device rd;

void Entity::update(const GameCmd*) {
    if (!alive_)
        return;

    if (rotSpeed_ != 0)
        setAngle(getAngle()+rotSpeed_);

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
        Bullet* b = dynamic_cast<Bullet*>(other.get());
        if (b) {
            b->getParent()->addXp(xp_);
        }
        Tank* t = dynamic_cast<Tank*>(other.get());
        if (t) {
            t->addXp(xp_);
        }
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
void  Entity::addXp(float xp) {
    xp_ += xp;
}
void Entity::setInertia(float i) {
    inertia_ = i;
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

void Entity::setShape(sf::Shape* shape) {
    shape_ = shape;
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



