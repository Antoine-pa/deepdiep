#include "Layout.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>


Layout::Layout(std::shared_ptr<World> world, std::shared_ptr<sf::RenderWindow> window):
    View::View(world, window) {
        sf::View view0;
        view0.setViewport(sf::FloatRect(0., 0., 0.5, 1.));
        views_.push_back(view0);
        addPlayerToLayout(world.get()->tanks[0]);
    }

void Layout::update(std::string overlayText, int x, int y) {
    View::update(players_[0].get(), views_[0], overlayText, x, y);
}

void Layout::addPlayerToLayout(std::shared_ptr<Tank> player) {
    players_.push_back(player);
    int nb_player = players_.size();
    for (int i = 0; i < nb_player; i++) {
        players_[i].get()->setViewport(views_[nb_player - 1 + i]);
    }
}
void Layout::removePlayerToLayout(std::shared_ptr<Tank> player) {
    players_.erase(
        std::remove(players_.begin(), players_.end(), player),
        players_.end()
    );
}
