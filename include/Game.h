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
#include<NeuralNetwork.h>
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
    void addNeuralNetwork(NeuralNetwork* net);
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
    std::vector<std::vector<sf::RectangleShape>> BlocksShape; //Contains the shape to draw on the screen
    std::vector<std::vector<int>> BlocksAvailable; //Contains a bool that indicates if the block is destructed or not
    std::vector<std::vector<sf::FloatRect>> BlocksBounds; //Contains the bounds to check collision with the ball
    NeuralNetwork* net;
};

#endif // GAME_H
