#ifndef INCLUDE_LAYOUT_HPP
#define INCLUDE_LAYOUT_HPP

#include "View.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class Layout: public View {
    private:
        std::vector<sf::View> views_;
        std::vector<std::shared_ptr<Tank>> players_;
    public:
        Layout(std::shared_ptr<World> world, std::shared_ptr<sf::RenderWindow> window);

        void update(std::string overlayText = "", int x=0, int y=0);

};
#endif
