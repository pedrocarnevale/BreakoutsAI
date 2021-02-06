#include<iostream>
#include<Ball.h>
#include<Game.h>
#include<Base.h>
int main()
{
    int WindowWidth = 840;
    int WindowHeight = 900;
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Breakouts Carnevale");
    float radius = 10;
    int FPS = 60;
    int BaseVel = 10;
    int BallVel = 10;
    int NumBlocksLine = 12;
    int NumBlocksColumn = 8;
    int BaseWidth = 100;
    int BaseHeight = 25;
    int BlockMargin = 5;
    int BlockOffset = 50;
    int BlockWidth = WindowWidth/NumBlocksLine - BlockMargin;
    int BlockHeight = 25;
    window.setFramerateLimit(FPS);
    Ball ball(&window, radius, BallVel);
    Base base(BaseVel, BaseWidth, BaseHeight, &window);
    Game game(NumBlocksLine, NumBlocksColumn, BlockMargin, BlockOffset, BlockWidth, BlockHeight, &window, &ball, &base);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        game.update();
        window.display();
    }

    return 0;
}
