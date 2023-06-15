#include<bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

class Menu {
public:
    Menu(sf::RenderWindow& window)
        : window(window), isPlaySelected(true)
    {
        // Load font
        if (!font.loadFromFile("Font.otf"))
        {
            cout << "Failed to load font." << endl;
            return;
        }

        // Create "Play" text
        playText.setString("Play");
        playText.setFont(font);
        playText.setCharacterSize(30);
        playText.setFillColor(sf::Color::White);
        playText.setPosition(window.getSize().x / 2 - playText.getGlobalBounds().width / 2, 200);

        // Create "Quit" text
        quitText.setString("Quit");
        quitText.setFont(font);
        quitText.setCharacterSize(30);
        quitText.setFillColor(sf::Color::White);
        quitText.setPosition(window.getSize().x / 2 - quitText.getGlobalBounds().width / 2, 250);

        // Set initial selection to "Play"
        playText.setFillColor(sf::Color::Red);
    }

    void draw()
    {
        window.clear();
        sf::Texture BG;
        if (!BG.loadFromFile("background.jpeg"))
        {
            cout << "Failed to load background image." << endl;
            return;
        }
        sf::Sprite backgroundSprite(BG);
        backgroundSprite.setScale(1, 1);
        backgroundSprite.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));

        // Center the sprite within the window
        backgroundSprite.setPosition(window.getSize().x / 2 - backgroundSprite.getLocalBounds().width / 2,
                                     window.getSize().y / 2 - backgroundSprite.getLocalBounds().height / 2);
        window.draw(backgroundSprite);

        window.draw(playText);
        window.draw(quitText);
        window.display();
    }

    bool handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return false;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    // Select "Play"
                    isPlaySelected = true;
                    playText.setFillColor(sf::Color::Red);
                    quitText.setFillColor(sf::Color::White);
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    // Select "Quit"
                    isPlaySelected = false;
                    playText.setFillColor(sf::Color::White);
                    quitText.setFillColor(sf::Color::Red);
                }
                else if (event.key.code == sf::Keyboard::Return)
                {
                    if (isPlaySelected)
                    {
                        cout << "Starting the game!" << endl;
                        return false;
                    }
                    else
                    {
                        cout << "Quitting the game." << endl;
                        window.close();
                        return false;
                    }
                }
            }
        }
        return true;
    }

private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text playText;
    sf::Text quitText;
    bool isPlaySelected;
    sf::Texture BG;
};


bool isFlying(sf::Sprite arrow, sf::Vector2f prev_pos){
    if(arrow.getPosition() != prev_pos)
        return true;
    else
        return false;
}


void runGame(sf::RenderWindow& window) {
    // Variables
    float rot_speed = 0.5;
    int mov_speed = 8;
    int wall_speed = 1;
    bool collided = false;
    sf::Vector2f direction;
    sf::Vector2f prev_pos;
    chrono::seconds pauseDuration(1);

    // Window settings
    const float grid_size = 50.f;

    // Textures
    sf::Texture texture_a;
    if (!texture_a.loadFromFile("arrow_.png")) {
        cerr << "Could not load texture" << endl;
        return;
    }

    sf::Texture texture_g;
    if (!texture_g.loadFromFile("goal_.png")) {
        cerr << "Could not load texture" << endl;
        return;
    }

    sf::Texture texture_bg;
    if (!texture_bg.loadFromFile("background.jpeg")) {
        cerr << "Could not load texture" << endl;
        return;
    }

    // Sprites
    sf::Vector2u arrow_size = texture_a.getSize();
    float arrow_scale_x = (grid_size * 1.5) / arrow_size.x;
    float arrow_scale_y = (grid_size * 1.5) / arrow_size.y;

    sf::Vector2u goal_size = texture_g.getSize();
    float goal_scale_x = (grid_size * 5) / goal_size.x;
    float goal_scale_y = (grid_size * 1.5) / goal_size.y;

    sf::Sprite arrow;
    arrow.setPosition(grid_size * 6, grid_size * 15);
    arrow.setScale(arrow_scale_x, arrow_scale_y);
    arrow.setTexture(texture_a);
    arrow.setOrigin(grid_size * 4.5, grid_size * 2);


    sf::Sprite goal;
    goal.setPosition(grid_size * 3.5, grid_size);
    goal.setScale(goal_scale_x, goal_scale_y);
    goal.setTexture(texture_g);

    sf::Sprite background;
    background.setTexture(texture_bg);
    background.setScale(1, 1);
    background.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));

    prev_pos = arrow.getPosition();

    // Wall movement
    const float wallLeft = 0.0f;
    const float wallRight = window.getSize().x - goal.getGlobalBounds().width;
    bool movingRight = true;

    // Variables for time and score
    sf::Clock clock;
    float timeElapsed = 0.0f;
    int score = 0;
    int remainingShots = 5;

    // Text settings
    sf::Font font;
    if (!font.loadFromFile("Font.otf")) {
        cerr << "Failed to load font." << endl;
        return;
    }

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(20);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(10, window.getSize().y - 30);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(window.getSize().x - 100, window.getSize().y - 30);

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        timeElapsed += elapsed.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //Collisions
        sf::FloatRect arrowBounds = arrow.getGlobalBounds();
        sf::FloatRect goalBounds = goal.getGlobalBounds();

        if (arrowBounds.left < 0 || arrowBounds.top < 0 ||
            arrowBounds.left + arrowBounds.width > window.getSize().x ||
            arrowBounds.top + arrowBounds.height > window.getSize().y)
        {
            direction = sf::Vector2f(0, 0);
            wall_speed = 0;
            collided = true;
            cout << "Missed!" << endl;
            this_thread::sleep_for(pauseDuration);
            arrow.setPosition(grid_size * 6, grid_size * 15);
            wall_speed = 1;
            collided = false;
            remainingShots--;

        }

        if (arrow.getGlobalBounds().intersects(goal.getGlobalBounds()))
        {
            // Calculate the hit position
            float intersectionWidth = arrowBounds.left + arrowBounds.width - goalBounds.left;

            collided = true;
            wall_speed = 0;

            if((intersectionWidth < 140 && intersectionWidth > 65) || (intersectionWidth > 196 && intersectionWidth < 275)){
                cout << "Hit for 5 points!" << endl;
                direction = sf::Vector2f(0, 0);
                score += 5;
                this_thread::sleep_for(pauseDuration);
                arrow.setPosition(grid_size * 6, grid_size * 15);
                wall_speed = 1;
                collided = false;
                remainingShots--;

            }
            else if(intersectionWidth >= 140 && intersectionWidth <= 196){
                cout << "Hit for 10 points!" << endl;
                direction = sf::Vector2f(0, 0);
                score += 10;
                this_thread::sleep_for(pauseDuration);
                arrow.setPosition(grid_size * 6, grid_size * 15);
                wall_speed = 1;
                collided = false;
                remainingShots--;
            }
        }

        //Sprite control
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && isFlying(arrow, prev_pos) == false && collided == false)
            if (arrow.getRotation() < 50 || arrow.getRotation() >= 310)
                arrow.rotate(rot_speed);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && isFlying(arrow, prev_pos) == false && collided == false)
            if (arrow.getRotation() <= 50 || arrow.getRotation() > 310)
                arrow.rotate(-rot_speed);

        if (remainingShots > 0) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isFlying(arrow, prev_pos) == false && collided == false) {

                direction.x = sin(arrow.getRotation() * 3.14159 / 180.0) * mov_speed;
                direction.y = -cos(arrow.getRotation() * 3.14159 / 180.0) * mov_speed;
            }
        }
        else {
            // Game over logic
            // Handle end of game, display final score or restart game, etc.
            cout << "Game over! Final score: " << score << endl;
            window.close();
        }

        //prev_pos = arrow.getPosition();
        arrow.move(direction);


        if (movingRight) {
            goal.move(wall_speed, 0);
            if (goal.getPosition().x >= wallRight) {
                movingRight = false;
            }
        }
        else {
            goal.move(-wall_speed, 0);
            if (goal.getPosition().x <= wallLeft) {
                movingRight = true;
            }
        }

        arrow.move(direction);

        window.clear(sf::Color::Red);

        window.draw(background);
        window.draw(arrow);
        window.draw(goal);

        timeText.setString("Time: " + to_string(static_cast<int>(timeElapsed)) + "s");
        scoreText.setString("Score: " + to_string(score));

        window.draw(timeText);
        window.draw(scoreText);

        window.display();
    }
}

int main(){
    const float grid_size = 50.f;
    sf::RenderWindow window(sf::VideoMode(grid_size * 12, grid_size * 18), "Arrows!");
    window.setFramerateLimit(144);

    Menu gameMenu(window);

    while (window.isOpen()) {
        gameMenu.draw();
        if (!gameMenu.handleEvents()) {
            if (window.isOpen()) {
                runGame(window);
            }
        }
    }

    return 0;
}
