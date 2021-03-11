#include<cmath>
#include<iostream>

#include "Game.h"

Game::Game(GameConfig Config, int Id, sf::RenderWindow* window):Id(Id),Score(0)
{
    this->BlockMargin = Config.BlockMargin;
    this->BlockOffset = Config.BlockOffset;
    this->BlockWidth = Config.BlockWidth;
    this->BlockHeight = Config.BlockHeight;
    this->NumBlocksColumn = Config.NumBlocksColumn;
    this->NumBlocksLine = Config.NumBlocksLine;

    sf::Color GameColor;
    GameColor.r = getRandomFloat(50,255);
    GameColor.g = getRandomFloat(50,255);
    GameColor.b = getRandomFloat(50,255);

    Ball* GameBall = new Ball(Config.Radius, Config.BallVel, GameColor, window);
    this->BreakoutsBall = GameBall;

    Base* GameBase = new Base(Config.BaseVel, Config.BaseWidth, Config.BaseHeight, GameColor, window);
    this->BreakoutsBase = GameBase;
    this->window = window;

    addNeuralNetwork(Config);
}

Game::Game()
{
    Ball* GameBall = new Ball();
    this->BreakoutsBall = GameBall;

    Base* GameBase = new Base();
    this->BreakoutsBase = GameBase;
}

Game::~Game()
{
    delete this->BreakoutsBall;
    delete this->BreakoutsBase;
}

std::vector<double> Game::getNewInputs()
{
    sf::CircleShape GameBall = BreakoutsBall->getGameBall();
    sf::RectangleShape GameBase = BreakoutsBase->getBaseShape();

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
    if (BreakoutsBall->collideBase(*BreakoutsBase))
        Score += 5;

    //Update ball
    BreakoutsBall->update(BreakoutsBase);

    //Update base using Neural Network
    std::vector<double> gameInputs = getNewInputs();
    net.setInputs(gameInputs);
    net.FeedFoward();
    //debugNeuralNetwork(net);

    std::vector<double> NetOutputs = net.getOutputs();
    double Left = NetOutputs[0];
    double Stationary = NetOutputs[1];
    double Right = NetOutputs[2];
    BreakoutsBase->update(Left, Stationary, Right);
}

void Game::draw()
{
    //Draw ball
    BreakoutsBall->draw();

    //Draw base
    BreakoutsBase->draw();
}

void Game::restart()
{
    BreakoutsBall->restart();
    BreakoutsBase->restart();
}

void Game::addNeuralNetwork(GameConfig Config)
{
    std::vector<double> inputsNN = getNewInputs();

    if ((int)inputsNN.size() != Config.NumInputsNN)
    {
        std::cout<<"Inputs Neural Network error";
        exit(0);
    }

    int numInputs = Config.NumInputsNN;
    NeuralNetwork netAI(numInputs, inputsNN, window);

    int numHiddenNeurons = Config.NumHiddenNeuronsNN;
    std::string activationF = "tanh";
    Layer hiddenLayer(numHiddenNeurons,activationF);
    netAI.addLayer(&hiddenLayer);

    int numOutputNeurons = Config.NumOutputNeuronsNN;
    Layer outputLayer(numOutputNeurons);
    netAI.addLayer(&outputLayer);

    this->net = netAI;
}

void Game::becomeNewGame(struct GameConfig Config, int Id, sf::RenderWindow* window)
{
    this->Id = Id;

    sf::Color GameColor;
    GameColor.r = getRandomFloat(50,255);
    GameColor.g = getRandomFloat(50,255);
    GameColor.b = getRandomFloat(50,255);

    BreakoutsBall->getGameBall().setFillColor(GameColor);
    BreakoutsBase->getBaseShape().setFillColor(GameColor);
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

int Game::getNumBlocksLine() const
{
    return NumBlocksLine;
}

int Game::getNumBlocksColumn() const
{
    return NumBlocksColumn;
}

int Game::getBlockMargin() const
{
    return BlockMargin;
}

int Game::getBlockOffset() const
{
    return BlockOffset;
}

int Game::getBlockWidth() const
{
    return BlockWidth;
}

int Game::getBlockHeight() const
{
    return BlockHeight;
}

Base* Game::getBreakoutsBase()
{
    return BreakoutsBase;
}

Ball* Game::getBreakoutsBall()
{
    return BreakoutsBall;
}

NeuralNetwork* Game::getNeuralNetwork()
{
    return &net;
}
