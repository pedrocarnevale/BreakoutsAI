#ifndef GAME_H
#define GAME_H

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<ctime>
#include<cmath>
#include "Ball.h"
class Game
{
public:
    Game(int NumBlocksLine,int NumBlocksColumn, int BlockMargin, int BlockOffset, int BlockWidth, int BlockHeight, sf::RenderWindow* window, Ball* ball, Base* base);
    virtual ~Game();
    int getNumBlocksLine();
    int getNumBlocksColumn();
    int getBlockMargin();
    int getBlockOffset();
    int getBlockWidth();
    int getBlockHeight();
    void update();
    void updateBall();
    void restart();
protected:

private:
    int NumBlocksLine;
    int NumBlocksColumn;
    int BlockMargin;
    int BlockOffset;
    int BlockWidth;
    int BlockHeight;
    sf::RenderWindow* window;
    Base* base;
    Ball* ball;
    std::pair<sf::RectangleShape,int> **Blocks;
    sf::FloatRect **BlocksBounds;
};

#endif // GAME_H
