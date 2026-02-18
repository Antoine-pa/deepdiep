#ifndef INCLUDE_LAYOUT_HPP
#define INCLUDE_LAYOUT_HPP

#include "World.hpp"
#include "View.hpp"


class Layout : public View {

public:
    Layout(std::shared_ptr<World> world, std::shared_ptr<sf::RenderWindow> window);

    void update(std::string overlayText = "", int x=0, int y=0);
};
#endif
