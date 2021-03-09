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
    void update();
    void draw();
    void restart();
    void addNeuralNetwork();

    void setScore(int newScore);
    int getScore();
    int getNumBlocksLine() const;
    int getNumBlocksColumn() const;
    int getBlockMargin() const;
    int getBlockOffset() const;
    int getBlockWidth() const;
    int getBlockHeight() const;
    Base* getBreakoutsBase();
    Ball* getBreakoutsBall();
    NeuralNetwork* getNeuralNetwork();
    std::vector<double> getNewInputs();

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
    NeuralNetwork net;
};
