#include "Menu.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>


Menu::Menu(const sf::Font& font, float x, float y, float width, float height, float spacing):
    font_(font), startX_(x), startY_(y), width_(width), height_(height), spacing_(spacing) {}

void Menu::addItem(const std::string& label, int size, float posX, float posY, bool centered, std::function<void()> callback) {
    MenuItem item;
    item.text.setFont(font_);
    item.text.setString(label);
    item.text.setCharacterSize(size);
    item.text.setFillColor(sf::Color::White);
    item.text.setStyle(sf::Text::Bold);
    item.centered = centered;
    
    auto bounds = item.text.getLocalBounds();
    float y = posY + items_.size() * spacing_;
    if (centered) item.text.setPosition(posX - bounds.width / 2, y);
    else item.text.setPosition(posX, y);
    
    item.bounds = item.text.getGlobalBounds();
    item.onClick = callback;

    // if (items_.size() > 0 && !items_[selectedIndex_].onClick && callback)
    //     selectedIndex_ = (int)(items_.size());

    items_.push_back(item);
}
void Menu::update(const sf::RenderWindow& window) {

    // --- Survol souris ---
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    for (int i = 0; i < (int)(items_.size()); i++) {
        if (items_[i].bounds.contains(mouse.x, mouse.y) && items_[i].onClick) {
            selectedIndex_ = i;
        }
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
void Menu::setSelectedIndex(int index) {
    selectedIndex_ = index % items_.size();
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
void Menu::updateMenuFromKeySet(KeySet* ks) {
    for (auto& item : items_) {
        if (item.text.getString().find("Move mode") != std::string::npos) {
            std::string text = "Move mode : ";
            if (ks->move_type == KeySet::Move::MKeys) text += "Keyboard";
            else text += "Joystick";
            item.text.setString(text);
        }
        if (item.text.getString().find("Aim mode") != std::string::npos) {
            std::string text = "Aim mode : ";
            if (ks->aim_type == KeySet::Aim::AMouse) text += "Mouse";
            else if (ks->aim_type == KeySet::Aim::AJoystick) text += "Joystick";
            else text += "Keyboard";
            item.text.setString(text);
        }

        if (item.text.getString().find("Move Up") != std::string::npos) {
            item.text.setString("Move Up : " + std::to_string((ks->up[0])));
        }
        if (item.text.getString().find("Move Down") != std::string::npos) {
            item.text.setString("Move Down : " + std::to_string((ks->down[0])));
        }
        if (item.text.getString().find("Move Left") != std::string::npos) {
            item.text.setString("Move Left : " + std::to_string((ks->left[0])));
        }
        if (item.text.getString().find("Move Right") != std::string::npos) {
            item.text.setString("Move Right : " + std::to_string((ks->right[0])));
        }
        if (item.text.getString().find("Fire") != std::string::npos) {
            item.text.setString("Fire : " + std::to_string((ks->fire[0])));
        }
        
        if (item.text.getString().find("View Up") != std::string::npos) {
            item.text.setString("View Up : " + std::to_string((ks->vup[0])));
        }
        if (item.text.getString().find("View Down") != std::string::npos) {
            item.text.setString("View Down : " + std::to_string((ks->vdown[0])));
        }
        if (item.text.getString().find("View Left") != std::string::npos) {
            item.text.setString("View Left : " + std::to_string((ks->vleft[0])));
        }
        if (item.text.getString().find("View Right") != std::string::npos) {
            item.text.setString("View Right : " + std::to_string((ks->vright[0])));
        }

        item.bounds = item.text.getGlobalBounds();
    }
}
