#pragma once

#include<cmath>
#include<ctime>

#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/window.hpp>

#include "Base.h"
#include "Ball.h"
#include "NeuralNetwork.h"
#include "utils.h"

class Game
{
public:
    Game(struct GameConfig Config, sf::RenderWindow* window);
    Game();
    ~Game();
    void update(enum Mode GameType);
    void draw(enum Mode GameType);
    void addNeuralNetwork(NeuralNetwork* net);

    int getScore();
    int getNumBlocksLine() const;
    int getNumBlocksColumn() const;
    int getBlockMargin() const;
    int getBlockOffset() const;
    int getBlockWidth() const;
    int getBlockHeight() const;

private:
    int Score;
    int BlockMargin;
    int BlockOffset;
    float BlockWidth;
    float BlockHeight;
    int NumBlocksLine;
    int NumBlocksColumn;
    sf::RenderWindow* window;
    Base* BreakoutsBase;
    Ball* BreakoutsBall;
    std::vector<std::vector<sf::RectangleShape>> BlocksShape; //Contains the shape to draw on the screen
    std::vector<std::vector<int>> BlocksAvailable; //Contains a bool that indicates if the block is destructed or not
    std::vector<std::vector<sf::FloatRect>> BlocksBounds; //Contains the bounds to check collision with the ball
    NeuralNetwork* net;
};
