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
        explicit Tank(World* world, sf::Vector2f pos, int team);
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

        sf::View getViewport();
        void setViewport(sf::View viewport);
        void setViewportSize(sf::Vector2f worldSize);
        sf::Vector2f getRelativeMousePos(sf::Vector2i mousePos);
        KeySet* getKeyset() const;
        void setKeyset(KeySet* ks);
        int getCanonsNumber();
        void addCanon();

        void reset();

    protected:
        int reload_time_ = 5;
        int damage_ = 6;
        float bullet_size_ = 2.f;
        float bullet_speed_ = 10.f;
        sf::Shape* empty_shape_;
        int canonsNumber;
    private:
        sf::View viewport_;
        int last_fire_ = 0;
        sf::Vector2f impulsion_ = sf::Vector2f(0.f, 0.f);      // impulsion is the inertia of the shots

        int goalScore = 100;
        float zoom_ = 1.;
        KeySet* ks_;
};

class MachineGun: public Tank {
    public:
        explicit MachineGun(World* world, sf::Vector2f pos, int team);
};
class Destroyer: public Tank {
    public:
        explicit Destroyer(World* world, sf::Vector2f pos, int team);
        ~Destroyer();
};
#endif
