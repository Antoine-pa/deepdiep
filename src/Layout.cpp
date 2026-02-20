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
        addPlayerToLayout(world->tanks[0]);
    }

void Layout::update(std::string overlayText, int x, int y) {
    window_->clear(sf::Color(225, 225, 225));
    for (auto &player : players_) {
        View::update(player.get(), overlayText, x, y);
    }
    window_->setView(window_->getDefaultView());
    // View::drawMainMenu();
}

void Layout::addPlayerToLayout(std::shared_ptr<Tank> player) {
    players_.push_back(player);
    int nb_player = players_.size();
    for (int i = 0; i < nb_player; i++) {
        players_[i]->setViewport(views_[nb_player - 1 + i]);
    }
}
void Layout::removePlayerToLayout(std::shared_ptr<Tank> player) {
    players_.erase(
        std::remove(players_.begin(), players_.end(), player),
        players_.end()
    );
    int nb_player = players_.size();
    for (int i = 0; i < nb_player; i++) {
        players_[i]->setViewport(views_[nb_player - 1 + i]);
    }
}
