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
    sf::Shape* shape_;

protected:
    /* Entity characteristics that should be set by the class */
    float bodyDamage_ = 1;
    float speed_ = 0;
    float inertia_ = 1;
    float hitRadius_;
    float maxHP_ = 1;
    float hp_ = 1;
    /* End of class-wide constants */

    int xp_ = 0;        // Correspond to the score of the entity or the experience earn after kill the entity
    sf::Vector2f position_;
    sf::Vector2f move_ = sf::Vector2f(0.f, 0.f);
    float rotSpeed_ = 0;
    float radius_;
    int team_;
    World* world_;
public:
    Entity(World* world, int team, float radius, sf::Shape* shape, sf::Vector2f pos, float angle, int xp);
    Entity(EntityPtr other) : Entity(*other) {}
    virtual ~Entity() = default;

    virtual void update(const GameCmd* command = nullptr);
    bool collides(const EntityPtr& other);
    virtual void hitBy(const EntityPtr& other);

    const sf::Vector2f& getPosition() const;
    float getAngle() const;
    void  setAngle(float angle);
    const sf::Vector2f& getDirection() const;
    void setDirection(sf::Vector2f dir);
    float getSpeed() const;
    void  setSpeed(float s);
    float getXp() const;
    void  addXp(float s);
    float getRotSpeed() const;
    void  setRotSpeed(float rs);
    float getRadius() const;
    sf::Shape* getShape() const;
    void setShape(sf::Shape* shape);
    int getTeam() const;
    float getHPRatio() const;
    bool isAlive() const;
    void kill();

    bool operator==(const Entity& other) const;
    bool operator!=(const Entity& other) const;
};


class Wall : public Entity {
    // sf::Vector2f position_; -> center of the wall
    // float angle_; -> angle of the wall
    // sf::Shape* shape_; -> shape of the wall (two types : circular and rectangular)
    // sf::Vector2f direction_; -> dimension of the wall if it's a rectangle
    // float radius_; -> radius of the wall if it's a circle
public:
    Wall(World* world, sf::Shape* shape, sf::Vector2f pos, float angle, float radius = 5., sf::Vector2f dir = sf::Vector2f(5., 5.));

    void hitBy(const EntityPtr& other) override { if (other->getAngle() == 0) {} };

    sf::Vector2f getNormal(sf::Vector2f CollisionPosition);
};


#endif
