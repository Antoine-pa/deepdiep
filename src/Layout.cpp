#include "Layout.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>


Layout::Layout(std::shared_ptr<World> world, std::shared_ptr<sf::RenderWindow> window):
    View::View(world, window) {
        views_[0].setViewport(sf::FloatRect(0., 0., 1., 1.));
        views_[1].setViewport(sf::FloatRect(0., 0., 0.5, 1.));
        views_[2].setViewport(sf::FloatRect(0.5, 0., 0.5, 1.));
        views_[3].setViewport(sf::FloatRect(0., 0., 0.5, 0.5));
        views_[4].setViewport(sf::FloatRect(0., 0.5, 0.5, 0.5));
        views_[5].setViewport(sf::FloatRect(0.5, 0., 0.5, 0.5));
        views_[6].setViewport(sf::FloatRect(0.5, 0.5, 0.5, 0.5));
        players_.push_back(world->tanks[0]);
        players_[0]->setViewport(views_[0]);
    }

void Layout::update(std::string overlayText, int x, int y) {
    View::update(players_[0].get(), overlayText, x, y);
}
