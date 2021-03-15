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
    Game(int Id);
    Game();
    void update();
    void decreaseBaseSize();
    void restart();
    void addNeuralNetwork();

    void setScore(int newScore);
    void setNeuralNetwork(NeuralNetwork newNet);
    int getId();
    int getScore();
    Base* getBreakoutsBase();
    Ball* getBreakoutsBall();
    NeuralNetwork* getNeuralNetwork();
    std::vector<double> getNewInputs();

private:
    int Id;
    int Score;
    Base BreakoutsBase;
    Ball BreakoutsBall;
    GameConfig Config;
    NeuralNetwork net;
};
