#include "Menu.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>


Menu::Menu(const sf::Font& font, float x, float y, float width, float height, float spacing):
    font_(font), startX_(x), startY_(y), width_(width), height_(height), spacing_(spacing) {}

void Menu::addItem(const std::string& label, float posX, float posY, bool centered, std::function<void()> callback) {
    MenuItem item;
    item.text.setFont(font_);
    item.text.setString(label);
    item.text.setCharacterSize(32);
    item.text.setFillColor(sf::Color::White);
    item.text.setStyle(sf::Text::Bold);
    item.centered = centered;
    
    auto bounds = item.text.getLocalBounds();
    float y = posY + items_.size() * spacing_;
    if (centered) item.text.setPosition(posX - bounds.width / 2, y);
    else item.text.setPosition(posX, y);
    
    item.bounds = item.text.getGlobalBounds();
    item.onClick = callback;

    if (items_.size() > 0 && !items_[selectedIndex_].onClick && callback)
        selectedIndex_ = (int)(items_.size());

    items_.push_back(item);
}
void Menu::update(const sf::RenderWindow& window) {
    // --- Navigation clavier ---
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    //     selectedIndex_ = (selectedIndex_ + 1) % items_.size();
    // }
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    //     selectedIndex_ = (selectedIndex_ - 1 + items_.size()) % items_.size();
    // }

    // --- Survol souris ---
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    for (int i = 0; i < (int)(items_.size()); i++) {
        if (items_[i].bounds.contains(mouse.x, mouse.y)) {
            selectedIndex_ = i;
        }
    }

    // --- Clic souris ---
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (items_[selectedIndex_].onClick)
            items_[selectedIndex_].onClick();
    }

    // --- Mise à jour visuelle ---
    for (int i = 0; i < (int)(items_.size()); i++) {
        if (i == selectedIndex_)
            items_[i].boundColor = sf::Color::Green;
        else
            items_[i].boundColor = sf::Color::Blue;
    }
}
void Menu::addSelectedIndex(bool sens) {
    if (sens) selectedIndex_ = (selectedIndex_ + 1) % items_.size();
    else selectedIndex_ = (selectedIndex_ - 1 + items_.size()) % items_.size();
    if (!items_[selectedIndex_].onClick) addSelectedIndex(sens);
}

void Menu::draw(sf::RenderWindow& window) {
    auto background = sf::RectangleShape(sf::Vector2f(width_, height_));
    background.setPosition(startX_, startY_);
    background.setFillColor(sf::Color(155, 155, 155, 155));
    window.draw(background);
    for (auto& item : items_) {
        auto rect = sf::RectangleShape(sf::Vector2f(item.bounds.width, item.bounds.height));
        rect.setPosition(item.bounds.left, item.bounds.top);
        rect.setFillColor(item.boundColor);
        rect.setOutlineColor(sf::Color(item.boundColor.r, item.boundColor.g, item.boundColor.b, 155));
        rect.setOutlineThickness(5);
        window.draw(rect);
        window.draw(item.text);
    }
}
void Menu::execute() {
    if (items_[selectedIndex_].onClick)
        items_[selectedIndex_].onClick();
}