#include "View.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

std::unordered_map<std::string, std::unique_ptr<sf::Shape>> View::shapes_;
sf::Font View::font;

void View::initAssets() {
    if (not View::shapes_.empty()) // Don't call this function twice
        return;

    // There is not much assets in this game: only the shapes (that are built manually below) and the font
    if (FILE *file = fopen("Monospace.ttf", "r")) {
        fclose(file); // That file exists, cool.
    } else {
        std::cout << "Impossible to load Monospace.ttf. Please launch the binary from a directory containing this file (either cd to the right location, or copy this file).\n";
    }
    if (!font.loadFromFile("Monospace.ttf")) {
        std::cout << "Runtime error: could not load Monospace.ttf font file\n";
        exit(1);
    }

    // If you want to have a shape made with several shapes, you should look at
    // https://github.com/SFML/SFML/wiki/Tutorial:-Drawable-Group

    // Create squares
    auto square = new sf::CircleShape(20.f, 4);
    square->setFillColor(sf::Color::Yellow);
    square->setOutlineColor(sf::Color(sf::Color::Yellow.r, sf::Color::Yellow.g, sf::Color::Yellow.b, 150));
    square->setOutlineThickness(3);
    square->setOrigin(20,20);
    registerShape("square", square);
    
    // Create triangles
    auto triangle = new sf::CircleShape(13.f, 3);
    triangle->setFillColor(sf::Color::Red);
    triangle->setOutlineColor(sf::Color(sf::Color::Red.r, sf::Color::Red.g, sf::Color::Red.b, 150));
    triangle->setOutlineThickness(3);
    triangle->setOrigin(13,13);
    registerShape("triangle", triangle);

    // Create pentagons
    auto pentagon = new sf::CircleShape(30.f, 5);
    pentagon->setFillColor(sf::Color::Blue);
    pentagon->setOutlineColor(sf::Color(sf::Color::Blue.r, sf::Color::Blue.g, sf::Color::Blue.b, 150));
    pentagon->setOutlineThickness(3);
    pentagon->setOrigin(30,30);
    registerShape("pentagon", pentagon);
    
    // Create hexagons
    auto hexagon = new sf::CircleShape(40.f, 6);
    hexagon->setFillColor(sf::Color::Cyan);
    hexagon->setOutlineColor(sf::Color(sf::Color::Cyan.r, sf::Color::Cyan.g, sf::Color::Cyan.b, 150));
    hexagon->setOutlineThickness(3);
    hexagon->setOrigin(40, 40);
    registerShape("hexagon", hexagon);
    
    // Bullets
    auto bullet = new sf::CircleShape(2.f, 20);
    bullet->setFillColor(sf::Color::Blue);
    bullet->setOutlineColor(sf::Color(sf::Color::Blue.r, sf::Color::Blue.g, sf::Color::Blue.b, 150));
    bullet->setOutlineThickness(1);
    bullet->setOrigin(2,2);
    registerShape("bullet", bullet);

    // Tank
    auto tank = new sf::RectangleShape(sf::Vector2f(23 ,12));
    tank->setFillColor(sf::Color(115, 115, 115));
    tank->setOutlineColor(sf::Color(38, 38, 38));
    tank->setOrigin(-4,6);
    tank->setOutlineThickness(1);
    registerShape("tank", tank);

    // Empty tank
    auto emptyTank = new sf::CircleShape(12.f, 20);
    emptyTank->setFillColor(sf::Color(128, 191, 255));
    emptyTank->setOutlineColor(sf::Color(51, 153, 255));
    emptyTank->setOutlineThickness(5);
    emptyTank->setOrigin(12,12);
    registerShape("empty tank", emptyTank);

    // Destroyer
    auto destroyer = new sf::RectangleShape(sf::Vector2f(28 ,16));
    destroyer->setFillColor(sf::Color(115, 115, 115));
    destroyer->setOutlineColor(sf::Color(38, 38, 38));
    destroyer->setOrigin(-14,8);
    destroyer->setOutlineThickness(1);
    registerShape("destroyer", destroyer);

    // Empty Destroyer
    auto emptyDestroyer = new sf::CircleShape(18.f, 20);
    emptyDestroyer->setFillColor(sf::Color(255, 191, 128));
    emptyDestroyer->setOutlineColor(sf::Color(255, 153, 51));
    emptyDestroyer->setOutlineThickness(5);
    emptyDestroyer->setOrigin(18,18);
    registerShape("empty destroyer", emptyDestroyer);

    // horizontal grid element
    auto hline = new sf::RectangleShape(sf::Vector2f(0, 2000));
    hline->setFillColor(sf::Color(145, 145, 145));
    hline->setOutlineColor(sf::Color(145, 145, 145));
    hline->setOutlineThickness(1);
    registerShape("hline", hline);
    auto vline = new sf::RectangleShape(*hline);
    vline->setSize(sf::Vector2f(2000, 0));
    registerShape("vline", vline);
}

View::View(std::shared_ptr<World> world, std::shared_ptr<sf::RenderWindow> window): world_(world), window_(window) {
    initAssets();
}

void View::registerShape(std::string name, sf::Shape *shape) {
    shapes_.insert({name, std::unique_ptr<sf::Shape>(shape)});
}
sf::Shape* View::getShape(std::string name) {
    initAssets();
    return shapes_.at(name).get();
}

void View::update(std::string overlayText, int x, int y) {
    window_->clear(sf::Color(225, 225, 225));

    sf::Vector2f padding = world_.get()->cameraPos_;
    sf::Vector2f offset = sf::Vector2f(world_.get()->windowWidth_ / 2, world_.get()->windowHeight_ / 2);
    float zoom = world_.get()->getCameraZoom();

    // Draw a grid
    auto hline = getShape("hline");
    for (int i=0; i<world_->getWidth();i += 50) {
        hline->setPosition((sf::Vector2f(i,0) - padding) * zoom + offset);
        hline->setScale(zoom, zoom);
        window_->draw(*hline);
    }
    auto vline = getShape("vline");
    for (int i=0; i<world_->getHeight();i += 50) {
        vline->setPosition((sf::Vector2f(0,i) - padding) * zoom + offset);
        vline->setScale(zoom, zoom);
        window_->draw(*vline);
    }

    for (auto const& e: world_->entities) {
        auto shape = e->getShape();
        shape->setRotation(e->getAngle());
        shape->setPosition((e->getPosition() - padding) * zoom + offset);
        shape->setScale(world_.get()->getCameraZoom(), world_.get()->getCameraZoom());
        window_->draw(*shape);
        auto hpRatio = e->getHPRatio();
        if (hpRatio>0 && hpRatio<1) {
            auto bar = sf::RectangleShape();
            auto progress = sf::RectangleShape();
            auto size = e->getRadius()*1.2;

            bar.setPosition((e->getPosition() + sf::Vector2f(-size/2, size) - padding) * zoom + offset);
            bar.setSize(sf::Vector2f(size, 4.));
            bar.setScale(world_.get()->getCameraZoom(), world_.get()->getCameraZoom());
            bar.setOutlineColor(sf::Color::Transparent);
            bar.setFillColor(sf::Color(127, 127, 127));
            window_->draw(bar);

            progress.setPosition((e->getPosition() + sf::Vector2f(-size/2, size) - padding) * zoom + offset);
            progress.setSize(sf::Vector2f(hpRatio*size, 4.));
            progress.setScale(world_.get()->getCameraZoom(), world_.get()->getCameraZoom());
            progress.setOutlineColor(sf::Color::Transparent);
            progress.setFillColor(sf::Color::Green);
            window_->draw(progress);

        }
    }
    for (auto const& s: world_->tanks) {
        auto shape = s->getEmptyShape();
        shape->setPosition(s->getPosition() - padding) * zoom + offset);
        // No rotation: that's a circle
        shape->setScale(world_.get()->getCameraZoom(), world_.get()->getCameraZoom());
        window_->draw(*shape);
    }

    if (not overlayText.empty()) {
        sf::Text displayedText(overlayText, font, 36);
        displayedText.setStyle(sf::Text::Bold);
        displayedText.setFillColor(sf::Color::Black);
        auto bounds = displayedText.getLocalBounds();
        displayedText.setPosition(x-bounds.width/2, y-bounds.height);
        window_->draw(displayedText);
    }

    // Drawing score bar
    if (!world_.get()->tanks.empty()) {
        auto bar = sf::RectangleShape();
        auto progress = sf::RectangleShape();
        auto size = world_.get()->getWindowWidth() / 2;
        auto ratio = world_.get()->tanks.at(0).get()->getXp() / world_.get()->tanks.at(0).get()->getGoalScore(); // on récupère le tank 0, s'il y en a plusieurs il faut regarder si c'est le bon...
        ratio = ratio > 1 ? 1 : ratio;

        bar.setPosition(sf::Vector2f((world_.get()->getWindowWidth() - size) / 2, world_.get()->getWindowHeight() - 20));
        bar.setSize(sf::Vector2f(size, 10.));
        bar.setOutlineColor(sf::Color::Transparent);
        bar.setFillColor(sf::Color(127, 127, 127));
        window_->draw(bar);
        
        progress.setPosition(sf::Vector2f((world_.get()->getWindowWidth() - size) / 2, world_.get()->getWindowHeight() - 20));
        progress.setSize(sf::Vector2f(size * ratio, 10.));
        progress.setOutlineColor(sf::Color::Transparent);
        progress.setFillColor(sf::Color::Green);
        window_->draw(progress);
    }

    // End of update
    window_->display();
}

World* View::getWorld() {
    return world_.get();
}