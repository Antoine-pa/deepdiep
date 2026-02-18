#ifndef INCLUDE_TANK_HPP
#define INCLUDE_TANK_HPP

#include "Entity.hpp"
#include "GameCmd.hpp"
#include "Bullet.hpp"
#include "View.hpp"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <math.h>
#include <memory>

class World;
class Bullet;

class Tank: public Entity {
    
    public:
        explicit Tank(World* world, sf::Vector2f pos);
        ~Tank();

        void fire();
        void update(const GameCmd* command) override;

        int getReloadTime();
        void setReloadTime(int reload);

        int getGoalScore();
        void setGoalScore(int newGS);

        int getDamage();
        void setDamage(int newDamage);
        float getZoom();
        
        sf::Shape* getEmptyShape();
        void setEmptyShape(sf::Shape*);

        sf::View getView();
        void setViewport(sf::View viewport);
    protected:
        int reload_time_ = 5;
        int damage_ = 1;
        sf::Shape* empty_shape_;
    private:
        sf::View viewport_;
        int last_fire_ = 0;
        sf::Vector2f impulsion_ = sf::Vector2f(0.f, 0.f);      // impulsion is the inertia of the shots

        int goalScore = 100;
        float zoom_ = 1.;
};

class Destroyer: public Tank {
    public:
        explicit Destroyer(World* world, sf::Vector2f pos);
        ~Destroyer();
};

#endif
