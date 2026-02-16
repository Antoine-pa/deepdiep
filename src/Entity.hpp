#ifndef INCLUDE_ENTITY_HPP
#define INCLUDE_ENTITY_HPP

#include "GameCmd.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <math.h>
#include <typeinfo>
#include <memory>

class World;
class Entity;

using EntityPtr = std::shared_ptr<Entity>;

class Entity {    
    float angle_; // Don't change it directly; use setAngle() to update direction_ accordingly
    sf::Vector2f direction_; // Only change it through setAngle()
    bool alive_ = true;
    short ttl_ = -1; // time to live. If negative, infinite
    sf::Shape* shape_;

public:
    // Objects only collide with the objects of the other teams
    enum class Team {
        ASTEROID, PLAYER
    };

protected:
    /* Entity characteristics that should be set by the class */
    float bodyDamage_ = 1;
    float speed_ = 0;
    float inertia_ = 1;
    float hitRadius_;
    float maxHP_ = 1;
    float hp_ = 1;
    /* End of class-wide constants */

    int xp_ = 0;
    sf::Vector2f position_;
    float moveX_ = 0;
    float moveY_ = 0;
    float rotSpeed_ = 0;
    float radius_;
    Entity::Team team_;
    World* world_;
public:
    Entity(World* world, Entity::Team team, float radius, sf::Shape* shape, sf::Vector2f pos, float angle, int xp);
    Entity(EntityPtr other) : Entity(*other) {}
    virtual ~Entity() = default;

    static EntityPtr makeSquare(World* world, float x, float y, float angle);
    static EntityPtr makeBullet(World* world, Entity::Team team, float x, float y, float angle);

    virtual void update(const GameCmd* command = nullptr);
    bool collides(const EntityPtr& other);
    void hitBy(const EntityPtr& other);

    const sf::Vector2f& getPosition() const;
    float getAngle() const;
    void  setAngle(float angle);
    const sf::Vector2f& getDirection() const;
    float getSpeed() const;
    void  setSpeed(float s);
    float getXp() const;
    void  setXp(float s);
    float getRotSpeed() const;
    void  setRotSpeed(float rs);
    float getRadius() const;
    sf::Shape* getShape() const;
    Entity::Team getTeam() const;
    float getHPRatio() const;
    bool isAlive() const;
    void kill();

    bool operator==(const Entity& other) const;
    bool operator!=(const Entity& other) const;
};

#endif
