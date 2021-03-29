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
    void update(Mode gameMode);
    void decreaseBaseSize();
    void restart();
    void addNeuralNetwork();

    void setScore(int newScore);
    void setStillAlive(bool newStillAlive);
    void setNeuralNetwork(NeuralNetwork newNet);
    int getId();
    int getScore();
    bool getStillAlive();
    Base* getBreakoutsBase();
    Ball* getBreakoutsBall();
    NeuralNetwork* getNeuralNetwork();
    std::vector<double> getNewInputs();

private:
    int Id;
    int Score;
    bool StillAlive;
    Base BreakoutsBase;
    Ball BreakoutsBall;
    NeuralNetwork net;
};
