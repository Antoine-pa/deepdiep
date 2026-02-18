#include "Layout.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>


Layout::Layout(std::shared_ptr<World> world, std::shared_ptr<sf::RenderWindow> window):
    View::View(world, window) {
        sf::View view0;
        view0.setViewport(sf::FloatRect(0., 0., 0.5, 1.));
        views_.push_back(view0);
        players_.push_back(world->tanks[0]);
        players_[0]->setViewport(views_[0]);
    }

void Layout::update(std::string overlayText, int x, int y) {
    View::update(overlayText, x, y);
}
