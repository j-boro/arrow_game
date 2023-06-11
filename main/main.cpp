#include<bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

bool isFlying(sf::Sprite arrow, sf::Vector2f prev_pos){
    if(prev_pos == arrow.getPosition())
        return false;
    else
        return true;
}

int main(){
    //variables
    float rot_speed = 0.5;
    int mov_speed = 8;
    int wall_speed = 1;
    sf::Vector2f direction;
    sf::Vector2f prev_pos;

    //window settings
    const float grid_size = 50.f;

    sf::RenderWindow window(sf::VideoMode(grid_size * 12, grid_size * 18), "Arrows!");
    window.setFramerateLimit(144);

    //textures
    sf::Texture texture_a;
    if (!texture_a.loadFromFile("arrow.jpg")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }

    sf::Texture texture_g;
    if (!texture_g.loadFromFile("goal.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }

    //sprites
    sf::Vector2u arrow_size = texture_a.getSize();
    float arrow_scale_x = (grid_size * 2) / arrow_size.x;
    float arrow_scale_y = (grid_size * 2) / arrow_size.y;

    sf::Vector2u goal_size = texture_g.getSize();
    float goal_scale_x = (grid_size * 5) / goal_size.x;
    float goal_scale_y = (grid_size * 1) / goal_size.y;

    sf::Sprite arrow;
    arrow.setPosition(grid_size * 6, grid_size * 16);
    arrow.setScale(arrow_scale_x, arrow_scale_y);
    arrow.setTexture(texture_a);
    arrow.setOrigin(grid_size * 2, grid_size * 2);

    sf::Sprite goal;
    goal.setPosition(grid_size * 3.5, grid_size);
    goal.setScale(goal_scale_x, goal_scale_y);
    goal.setTexture(texture_g);

    prev_pos = arrow.getPosition();
    //wall movement
    const float wallLeft = 0.0f;
    const float wallRight = window.getSize().x - goal.getGlobalBounds().width;
    bool movingRight = true;

    while(window.isOpen()){

        //check if window closed
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //keyboard inputs
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && isFlying(arrow, prev_pos) == false)
            if(arrow.getRotation() < 50 || arrow.getRotation() >= 310)
                arrow.rotate(rot_speed);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && isFlying(arrow, prev_pos) == false)
            if(arrow.getRotation() <= 50 || arrow.getRotation() > 310)
                arrow.rotate(-rot_speed);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isFlying(arrow, prev_pos) == false){
            direction.x = sin(arrow.getRotation() * 3.14159 / 180.0) * mov_speed;
            direction.y = -cos(arrow.getRotation() * 3.14159 / 180.0) * mov_speed;
        }

        //wall movement
        if (movingRight)
        {
            goal.move(wall_speed, 0);
            if (goal.getPosition().x >= wallRight)
            {
                movingRight = false;
            }
        }
        else
        {
            goal.move(-wall_speed, 0);
            if (goal.getPosition().x <= wallLeft)
            {
                movingRight = true;
            }
        }

        arrow.move(direction);

        window.clear(sf::Color::White);

        window.draw(arrow);
        window.draw(goal);

        window.display();
    }
}
