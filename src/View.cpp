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

    // Body tank
    auto bodyTank = new sf::CircleShape(12.f, 20);
    bodyTank->setOutlineThickness(5);
    bodyTank->setOrigin(12,12);
    registerShape("body tank", bodyTank);

    // MachineGun
    auto machinegun = new sf::RectangleShape(sf::Vector2f(23, 22));
    machinegun->setFillColor(sf::Color(115, 115, 115));
    machinegun->setOutlineColor(sf::Color(38, 38, 38));
    machinegun->setOrigin(-11,11);
    machinegun->setOutlineThickness(1);
    registerShape("machine gun", machinegun);

    // Body MachineGun
    auto bodyMachinegun = new sf::CircleShape(14.f, 20);
    bodyMachinegun->setOutlineThickness(5);
    bodyMachinegun->setOrigin(14,14);
    registerShape("body machine gun", bodyMachinegun);

    // Destroyer
    auto destroyer = new sf::RectangleShape(sf::Vector2f(23, 22));
    destroyer->setFillColor(sf::Color(115, 115, 115));
    destroyer->setOutlineColor(sf::Color(38, 38, 38));
    destroyer->setOrigin(-11,11);
    destroyer->setOutlineThickness(1);
    registerShape("destroyer", destroyer);

    // Body Destroyer
    auto bodyDestroyer = new sf::CircleShape(18.f, 20);
    bodyDestroyer->setOutlineThickness(5);
    bodyDestroyer->setOrigin(18,18);
    registerShape("body destroyer", bodyDestroyer);

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

sf::FloatRect drawText(sf::RenderWindow* win, sf::Font font, std::string text, int x, int y, int size, bool center=false) {
    sf::Text displayedText(text, font, size);
    displayedText.setStyle(sf::Text::Bold);
    displayedText.setFillColor(sf::Color::Black);
    auto bounds = displayedText.getLocalBounds();
    if (center)
        displayedText.setPosition(x-bounds.width / 2, y-bounds.height / 2);
    else displayedText.setPosition(x, y);
    win->draw(displayedText);
    return bounds;
}

void View::update(Tank* tank, std::string overlayText, int x, int y) {
    sf::View view = tank->getViewport();
    view.setCenter(world_->getCameraPos(tank));
    view.setSize(sf::Vector2f(world_->windowWidth_ * view.getViewport().width / tank->getZoom(), world_->windowHeight_ * view.getViewport().height / tank->getZoom()));
    window_->setView(view);
 
    // Draw a grid
    auto hline = getShape("hline");
    for (int i=0; i<=world_->getWidth();i += 50) {
        hline->setPosition(sf::Vector2f(i,0));
        window_->draw(*hline);
    }
    auto vline = getShape("vline");
    for (int i=0; i<=world_->getHeight();i += 50) {
        vline->setPosition(sf::Vector2f(0,i));
        window_->draw(*vline);
    }

    for (auto const& e: world_->entities) {
        auto shape = e->getShape();
        shape->setRotation(e->getAngle());
        shape->setPosition(e->getPosition());
        if (shape == getShape("bullet")) shape->setScale(e->getRadius(), e->getRadius());
        if (e->getTeam() > 0 && shape == getShape("bullet") && e->getTeam() != tank->getTeam()) {
            shape->setFillColor(sf::Color::Red);
            shape->setOutlineColor(sf::Color(sf::Color::Red.r, sf::Color::Red.g, sf::Color::Red.b, 150));
        }
        else if (e->getTeam() > 0 && shape == getShape("bullet") && e->getTeam() == tank->getTeam()) {
            shape->setFillColor(sf::Color::Blue);
            shape->setOutlineColor(sf::Color(sf::Color::Blue.r, sf::Color::Blue.g, sf::Color::Blue.b, 150));
        }
        window_->draw(*shape);
        auto hpRatio = e->getHPRatio();
        if (hpRatio>0 && hpRatio<1) {
            auto bar = sf::RectangleShape();
            auto progress = sf::RectangleShape();
            auto size = e->getRadius()*1.2;

            bar.setPosition(e->getPosition() + sf::Vector2f(-size/2, size));
            bar.setSize(sf::Vector2f(size, 4.));
            bar.setOutlineColor(sf::Color::Transparent);
            bar.setFillColor(sf::Color(127, 127, 127));
            window_->draw(bar);

            progress.setPosition(e->getPosition() + sf::Vector2f(-size/2, size));
            progress.setSize(sf::Vector2f(hpRatio*size, 4.));
            progress.setOutlineColor(sf::Color::Transparent);
            progress.setFillColor(sf::Color::Green);
            window_->draw(progress);

        }
    }
    for (auto const& s: world_->tanks) {
        int cNumber = s->getCanonsNumber();
        auto shape = s->getShape();
        for(int i = 0; i<cNumber; i++) {
            shape->setRotation(s->getAngle()+360/cNumber*i);
            shape->setPosition(s->getPosition());
            window_->draw(*shape);
        }
        shape = s->getEmptyShape();
        shape->setPosition(s->getPosition());
        if (s->getTeam() != tank->getTeam()) {
            shape->setFillColor(sf::Color(255, 41, 28));
            shape->setOutlineColor(sf::Color(255, 81, 71));
        }
        else {
            shape->setFillColor(sf::Color(128, 191, 255));
            shape->setOutlineColor(sf::Color(51, 153, 255));
        }
        // No rotation: that's a circle
        window_->draw(*shape);
    }

    if (not overlayText.empty()) {
        drawText(window_.get(), font, overlayText, x, y, 36, true);
        // sf::Text displayedText(overlayText, font, 36);
        // displayedText.setStyle(sf::Text::Bold);
        // displayedText.setFillColor(sf::Color::Black);
        // auto bounds = displayedText.getLocalBounds();
        // displayedText.setPosition(x-bounds.width/2, y-bounds.height);
        // window_->draw(displayedText);
    }

    // Drawing score bar
    auto bar = sf::RectangleShape();
    auto progress = sf::RectangleShape();
    auto size = world_->getWindowWidth() * tank->getViewport().getViewport().width / 2;
    auto ratio = tank->getXp() / tank->getGoalScore();
    ratio = ratio > 1 ? 1 : ratio;

    bar.setPosition(view.getCenter() + sf::Vector2f(- size / 2, world_->getWindowHeight() * tank->getViewport().getViewport().height / 2 - 20) / tank->getZoom());
    bar.setSize(sf::Vector2f(size, 10.) / tank->getZoom());
    bar.setOutlineColor(sf::Color::Transparent);
    bar.setFillColor(sf::Color(127, 127, 127));
    window_->draw(bar);
    
    progress.setPosition(view.getCenter() + sf::Vector2f(- size / 2, world_->getWindowHeight() * tank->getViewport().getViewport().height / 2 - 20) / tank->getZoom());
    progress.setSize(sf::Vector2f(size * ratio, 10.) / tank->getZoom());
    progress.setOutlineColor(sf::Color::Transparent);
    progress.setFillColor(sf::Color::Green);
    window_->draw(progress);

    if (tank->getKeyset()->aim_type == KeySet::Aim::AKeys) {
        auto target = sf::CircleShape(3.f, 20);
        target.setFillColor(sf::Color::Red);
        target.setPosition(tank->getPosition() + (sf::Vector2f)(tank->getKeyset()->keyMousePos_));
        window_->draw(target);
    }

}

void View::drawMainMenu() {
    auto screen = sf::RectangleShape(sf::Vector2f(world_->windowWidth_, world_->windowHeight_));
    screen.setFillColor(sf::Color(0, 0, 0, 150));
    window_->draw(screen);

    int height = world_->windowHeight_ / 3;
    sf::FloatRect title = drawText(window_.get(), font, "DeepDiep", world_->windowWidth_/2, height, 72, true);
    height += title.height * 2;
    sf::FloatRect addPlayer = drawText(window_.get(), font, "Add player", world_->windowWidth_/2, height, 40, true);
    height += addPlayer.height;
    sf::FloatRect settings =  drawText(window_.get(), font, "Settings", world_->windowWidth_/2, height, 40, true);
    height += settings.height;
    sf::FloatRect start = drawText(window_.get(), font, "Start the game", world_->windowWidth_/2, height, 40, true);
    height += start.height;

    (void)addPlayer;
    (void)settings;
    (void)start;
    (void)title;

}
void View::drawDeadMenu(Tank* tank) {
    (void)tank;
}
void View::drawSettingsMenu(Tank* tank) {
    (void)tank;
}

World* View::getWorld() {
    return world_.get();
}
