#include "world.hxx"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <error/error.hxx>

// #include "world.hxx"

int main(){
    using namespace std;
    std::cout << "hello world!" << std::endl;
    sf::RenderWindow window(sf::VideoMode(800,600), "Submarines!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Font font;
    
    if (!font.loadFromFile("../assets/fonts/LiberationSans-Regular.ttf")){
        ERR("unable to load font.\n");
    }

    sf::Text text;
    text.setFont(font);

    text.setString("This is a game about submarines.");
    text.setCharacterSize(24);
    text.setColor(sf::Color::Red);
    // text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    // inside the main loop, between window.clear() and window.display()
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        // window.draw(shape);
        window.draw(text);
        window.display();

    }


    return 0;
}