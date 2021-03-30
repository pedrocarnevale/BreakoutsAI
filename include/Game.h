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
    void update(Mode gameMode, std::vector<std::vector<int>> BlocksAvailable);
    void decreaseBaseSize();
    void restart();
    void addNeuralNetwork();

    void setScore(int newScore);
    void setScoreMemory(std::vector<int>& newScoreMemory);
    void setStillAlive(bool newStillAlive);
    void setNeuralNetwork(NeuralNetwork newNet);
    int getId();
    int getScore();
    float getAverageScore();
    bool getStillAlive();
    Base* getBreakoutsBase();
    Ball* getBreakoutsBall();
    NeuralNetwork* getNeuralNetwork();
    std::vector<double> getNewInputs(std::vector<std::vector<int>> BlocksAvailable);
    std::vector<int> getScoreMemory();

private:
    int Id;
    int Score;
    std::vector<int> scoreMemory;
    bool StillAlive;
    Base BreakoutsBase;
    Ball BreakoutsBall;
    NeuralNetwork net;
};
