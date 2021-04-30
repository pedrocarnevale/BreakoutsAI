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

    this->StillAlive = true;
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

std::vector<double> Game::getNewInputs(std::vector<std::vector<int>> BlocksAvailable)
{
    std::vector<int> numBlocksColumns(GameConfig::NumBlocksLine, 0);

    for(int i = 0; i < GameConfig::NumBlocksLine; i++)
    {
        for(int j = 0; j < GameConfig::NumBlocksColumn; j++)
        {
            if (BlocksAvailable[i][j])
                numBlocksColumns[i]++;
        }
    }
    sf::CircleShape GameBall = BreakoutsBall.getGameBall();
    sf::RectangleShape GameBase = BreakoutsBase.getBaseShape();

    double Radius = GameBall.getRadius();
    double BaseWidth = GameBase.getSize().x;

    double BallPositionX = GameBall.getPosition().x + Radius;
    double BallPositionY = GameConfig::WindowHeight - (GameBall.getPosition().y + Radius);
    double BasePositionX = GameBase.getPosition().x + BaseWidth / 2;
    double BasePositionY = GameConfig::WindowHeight - (GameBase.getPosition().y + GameBase.getSize().y / 2);

    std::vector<double> netInputs;

    netInputs.push_back(BallPositionX);
    netInputs.push_back(BasePositionX);
    netInputs.push_back(BallPositionY - BasePositionY);

    std::vector<double> NormalizedNetInputs = standardScaler(netInputs);
    return NormalizedNetInputs;
}

void Game::update(Mode gameMode, std::vector<std::vector<int>> BlocksAvailable)
{
    //If collided increase 5 points
    if (BreakoutsBall.collideBase(BreakoutsBase))
    {
        Score += GameConfig::CollidedBaseBonus;

        //Increase Difficulty
        if (Score >= GameConfig::DecreaseBaseSizeLimit && gameMode == Mode::TRAINING)
            decreaseBaseSize();
    }

    //Update ball
    BreakoutsBall.update();

    //Update base using Neural Network
    std::vector<double> gameInputs = getNewInputs(BlocksAvailable);
    net.setInputs(gameInputs);
    net.FeedFoward();

    std::vector<double> NetOutputs = net.getOutputs();
    double Left = NetOutputs[0];
    double Stationary = NetOutputs[1];
    double Right = NetOutputs[2];
    BreakoutsBase.update(Left, Stationary, Right);

    if (BreakoutsBase.getDirection() != Direction::STATIONARY)
        Score -= GameConfig::MoveBasePenalty;
}

void Game::decreaseBaseSize()
{
    sf::Vector2f newSize = BreakoutsBase.getBaseShape().getSize();
    newSize.x *= GameConfig::DecreaseFraction;
    BreakoutsBase.getBaseShape().setSize(newSize);
}

void Game::restart()
{
    BreakoutsBall.restart();
    BreakoutsBase.restart();
}

void Game::addNeuralNetwork()
{
    std::vector<std::vector<int>> BlocksAvailable;
    BlocksAvailable.resize(GameConfig::NumBlocksLine);

    for(int i = 0; i < GameConfig::NumBlocksLine; i++)
        BlocksAvailable[i].push_back(3);

    std::vector<double> inputsNN = getNewInputs(BlocksAvailable);

    if ((int)inputsNN.size() != GameConfig::NumInputsNN)
    {
        std::cout<<"Inputs Neural Network error";
        exit(0);
    }

    int numInputs = GameConfig::NumInputsNN;
    NeuralNetwork netAI(numInputs, inputsNN);

    int numHiddenNeurons = GameConfig::NumHiddenNeuronsNN;
    std::string activationF = "tanh";
    Layer hiddenLayer(numHiddenNeurons,activationF);
    netAI.addLayer(&hiddenLayer);

    int numOutputNeurons = GameConfig::NumOutputNeuronsNN;
    Layer outputLayer(numOutputNeurons);
    netAI.addLayer(&outputLayer);

    this->net = netAI;
}

void Game::setScore(int newScore)
{
    this->Score = newScore;
}

void Game::setScoreMemory(std::vector<int>& newScoreMemory)
{
    this->scoreMemory = newScoreMemory;
}

void Game::setStillAlive(bool newStillAlive)
{
    this->StillAlive = newStillAlive;
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

float Game::getAverageScore()
{
    float sum = 0;
    for (int i = 0; i < (int)scoreMemory.size(); i++)
    {
        sum += scoreMemory[i];
    }

    return sum / static_cast<float>(scoreMemory.size());
}

bool Game::getStillAlive()
{
    return StillAlive;
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

std::vector<int> Game::getScoreMemory()
{
    return this->scoreMemory;
}
