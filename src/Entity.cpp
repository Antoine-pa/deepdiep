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
    Wall* w = dynamic_cast<Wall*>(other.get());
    if (w) {
        Bullet* b = dynamic_cast<Bullet*>(this);
        if (b) {
            position_ -= direction_ * speed_;
            sf::Vector2f normal = w->getNormal(position_);
            world_->push(std::shared_ptr<Entity>(
                new Bullet(world_, b->getParent(), position_.x, position_.y, atan2f(normal.y, normal.x), speed_, 25)));
        }
        sf::Vector2f normal = w->getNormal(position_);
        position_ -= move_;
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
void Entity::setDirection(sf::Vector2f dir) {
    direction_ = dir;
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



Wall::Wall(World* world, sf::Shape* shape, sf::Vector2f pos, float angle, float radius, sf::Vector2f dir):
    Entity(world, -1, radius, shape, pos, angle, 0) {
        setDirection(dir);
        bodyDamage_ = 1;
    }

sf::Vector2f Wall::getNormal(sf::Vector2f CollisionPosition) {
    sf::Vector2f d = CollisionPosition - position_;
    if (getShape() == View::getShape("circular wall")) {
        float len = sqrtf(d.x * d.x + d.y * d.y);
        return d / len;
    }

    sf::Vector2f ux = sf::Vector2f(cosf(getAngle()), sinf(getAngle()));
    sf::Vector2f uy = sf::Vector2f(-sinf(getAngle()), cosf(getAngle()));

    // Projection dans l'espace local
    float px = d.x * ux.x + d.y * ux.y;
    float py = - d.x * uy.x + d.y * uy.y;

    float dx = getDirection().x / 2 - std::abs(px);
    float dy = getDirection().y / 2 - std::abs(py);

    sf::Vector2f localNormal;

    if (dx < dy) {
        // Face verticale
        localNormal = (px > 0) ? sf::Vector2f(1, 0) : sf::Vector2f(-1, 0);
    } else {
        // Face horizontale
        localNormal = (py > 0) ? sf::Vector2f(0, 1) : sf::Vector2f(0, -1);
    }

    // Repasser en espace global
    sf::Vector2f globalNormal =
        localNormal.x * ux +
        localNormal.y * uy;

    return globalNormal;
}
