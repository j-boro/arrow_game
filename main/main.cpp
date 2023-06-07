#include<bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

int main(){
    //window settings
    const float grid_size = 50.f;

    sf::RenderWindow window(sf::VideoMode(grid_size * 12, grid_size * 18), "Arrows!");
    window.setFramerateLimit(144);

    //textures
    sf::Texture texture_g;
    if (!texture_g.loadFromFile("goal.jpg")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }

    //sprites
    sf::Vector2u goal_size = texture_g.getSize();
    float goal_scale_x = (grid_size * 2) / goal_size.x;
    float goal_scale_y = (grid_size * 2) / goal_size.y;

    sf::Sprite arrow;
    arrow.setPosition(grid_size * 5, grid_size * 15);
    arrow.setScale(goal_scale_x, goal_scale_y);
    arrow.setTexture(texture_g);

    while(window.isOpen()){

        //check if window closed
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        window.draw(arrow);

        window.display();
    }
}
