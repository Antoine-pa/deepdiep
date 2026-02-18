#include "Layout.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>


Layout::Layout(std::shared_ptr<World> world, std::shared_ptr<sf::RenderWindow> window):
    View::View(world, window) {}

void Layout::update(std::string overlayText, int x, int y) {
    View::update(overlayText, x, y);
}
