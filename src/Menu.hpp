#ifndef INCLUDE_MENU_HPP
#define INCLUDE_MENU_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>


struct MenuItem {
    sf::Text text;
    bool centered;
    sf::FloatRect bounds;
    sf::Color boundColor = sf::Color::Blue;
    std::function<void()> onClick;
};
class Menu {
public:
    Menu(const sf::Font& font, float x, float y, float width, float height, float spacing);

    void addItem(const std::string& label, float posX, float posY, bool centered, std::function<void()> callback);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void execute();
    void addSelectedIndex(bool sens);

private:
    std::vector<MenuItem> items_;
    const sf::Font& font_;
    float startX_, startY_, width_, height_, spacing_;
    int selectedIndex_ = 0;
};

#endif
