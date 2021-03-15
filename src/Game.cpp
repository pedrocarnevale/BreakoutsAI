#include<cmath>
#include<iostream>

#include "Game.h"

Game::Game(int Id):Id(Id),Score(0)
{
    sf::Color GameColor;
    GameColor.r = getRandomFloat(50,255);
    GameColor.g = getRandomFloat(50,255);
    GameColor.b = getRandomFloat(50,255);

    Ball GameBall(GameColor);
    this->BreakoutsBall = GameBall;

    Base GameBase(GameColor);
    this->BreakoutsBase = GameBase;

    addNeuralNetwork();
}

Game::Game()
{
    Id = 0;
    Score = 0;

    sf::Color GameColor;
    GameColor.r = getRandomFloat(50,255);
    GameColor.g = getRandomFloat(50,255);
    GameColor.b = getRandomFloat(50,255);

    Ball GameBall(GameColor);
    this->BreakoutsBall = GameBall;

    Base GameBase(GameColor);
    this->BreakoutsBase = GameBase;

    addNeuralNetwork();

}

std::vector<double> Game::getNewInputs()
{
    sf::CircleShape GameBall = BreakoutsBall.getGameBall();
    sf::RectangleShape GameBase = BreakoutsBase.getBaseShape();

    double Radius = GameBall.getRadius();
    double BaseWidth = GameBase.getSize().x;

    double BallPositionX = GameBall.getPosition().x + Radius;
    double BallPositionY = GameBall.getPosition().y + Radius;
    double BasePositionX = GameBase.getPosition().x + BaseWidth / 2;

    std::vector<double> netInputs = {BallPositionX, BasePositionX, BallPositionY};
    std::vector<double> NormalizedNetInputs = standardScaler(netInputs);
    return NormalizedNetInputs;
}

void Game::update()
{
    //If collided increase 5 points
    if (BreakoutsBall.collideBase(BreakoutsBase))
    {
        Score += 5;

        //Increase Difficulty
        if (Score >= 50)
            decreaseBaseSize();
    }

    //Update ball
    BreakoutsBall.update();

    //Update base using Neural Network
    std::vector<double> gameInputs = getNewInputs();
    net.setInputs(gameInputs);
    net.FeedFoward();
    //debugNeuralNetwork(net);

    std::vector<double> NetOutputs = net.getOutputs();
    double Left = NetOutputs[0];
    double Stationary = NetOutputs[1];
    double Right = NetOutputs[2];
    BreakoutsBase.update(Left, Stationary, Right);
}

void Game::decreaseBaseSize()
{
    sf::Vector2f newSize = BreakoutsBase.getBaseShape().getSize();
    newSize.x *= Config.DecreaseFraction;
    BreakoutsBase.getBaseShape().setSize(newSize);
}

void Game::restart()
{
    BreakoutsBall.restart();
    BreakoutsBase.restart();
}

void Game::addNeuralNetwork()
{
    std::vector<double> inputsNN = getNewInputs();

    if ((int)inputsNN.size() != Config.NumInputsNN)
    {
        std::cout<<"Inputs Neural Network error";
        exit(0);
    }

    int numInputs = Config.NumInputsNN;
    NeuralNetwork netAI(numInputs, inputsNN);

    int numHiddenNeurons = Config.NumHiddenNeuronsNN;
    std::string activationF = "tanh";
    Layer hiddenLayer(numHiddenNeurons,activationF);
    netAI.addLayer(&hiddenLayer);

    int numOutputNeurons = Config.NumOutputNeuronsNN;
    Layer outputLayer(numOutputNeurons);
    netAI.addLayer(&outputLayer);

    this->net = netAI;
}

void Game::setScore(int newScore)
{
    this->Score = newScore;
}

void Game::setNeuralNetwork(NeuralNetwork newNet)
{
    this->net = newNet;
}

int Game::getId()
{
    return Id;
}

int Game::getScore()
{
    return Score;
}

Base* Game::getBreakoutsBase()
{
    return &BreakoutsBase;
}

Ball* Game::getBreakoutsBall()
{
    return &BreakoutsBall;
}

NeuralNetwork* Game::getNeuralNetwork()
{
    return &net;
}
