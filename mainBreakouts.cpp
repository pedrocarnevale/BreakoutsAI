#include<iostream>
#include<random>

#include "Ball.h"
#include "Base.h"
#include "Game.h"
#include "Environment.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(GameConfig::WindowWidth, GameConfig::WindowHeight), "BreakoutsAI");
    window.setFramerateLimit(GameConfig::FPS);

    //Construct environment
    Environment environment(&window);

    //Update environment
    environment.update();
    return 0;
}
