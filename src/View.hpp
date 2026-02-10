#ifndef INCLUDE_VIEW_HPP
#define INCLUDE_VIEW_HPP

#include "World.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <memory>
#include <unordered_map>

class View {
    std::shared_ptr<World> world_;
    std::shared_ptr<sf::RenderWindow> window_;

    static void initAssets();
    static std::unordered_map<std::string, std::unique_ptr<sf::Shape>> shapes_;
    static sf::Font font;

public:
    View(std::shared_ptr<World> world, std::shared_ptr<sf::RenderWindow> window);

    static sf::Shape* getShape(std::string name);
    static void registerShape(std::string name, sf::Shape* shape);

    void update(std::string overlayText = "", int x=0, int y=0);
};
#endif
