#include<cmath>
#include<iostream>

#include "Game.h"

Game::Game(struct GameConfig Config, sf::RenderWindow* window)
{
    this->BlockMargin = Config.BlockMargin;
    this->BlockOffset = Config.BlockOffset;
    this->BlockWidth = Config.BlockWidth;
    this->BlockHeight = Config.BlockHeight;
    this->NumBlocksColumn = Config.NumBlocksColumn;
    this->NumBlocksLine = Config.NumBlocksLine;
    this->Score = 0;

    sf::Color GameColor;
    GameColor.r = getRandomFloat(50,255);
    GameColor.g = getRandomFloat(50,255);
    GameColor.b = getRandomFloat(50,255);

    Ball* GameBall = new Ball(Config.Radius, Config.BallVel, GameColor, window);
    this->BreakoutsBall = GameBall;

    Base* GameBase = new Base(Config.BaseVel, Config.BaseWidth, Config.BaseHeight, GameColor, window);
    this->BreakoutsBase = GameBase;
    this->window = window;

    addNeuralNetwork();
}

Game::Game()
{

}

Game::~Game()
{
    delete this->BreakoutsBall;
    delete this->BreakoutsBase;
}

void Game::update()
{
    //Update ball
    BreakoutsBall->update(BreakoutsBase);

    //Update base using Neural Network
    sf::CircleShape GameBall = BreakoutsBall->getGameBall();
    sf::RectangleShape GameBase = BreakoutsBase->getBaseShape();

    double BallPositionX = GameBall.getPosition().x;
    double BallPositionY = GameBall.getPosition().y;
    double BasePositionX = GameBase.getPosition().x;
    double BasePositionY = GameBase.getPosition().y;

    double Radius = GameBall.getRadius();
    double BaseWidth = GameBase.getSize().x;

    double DeltaY = BasePositionY - BallPositionY;
    double DeltaX = (BasePositionX + BaseWidth/2) - (BallPositionX + Radius);
    std::vector<double> inputs = {DeltaX, DeltaY};

    net.FeedFoward();

    //std::vector<double> NetOutputs = net.getOutputs();
    //double Left = NetOutputs[0];
    //double Stationary = NetOutputs[1];
    //double Right = NetOutputs[2];
    //BreakoutsBase->update(Left, Stationary, Right);
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
    Score = 0;
}

void Game::addNeuralNetwork()
{
    int numInputs = 2;
    std::vector<double> inputs= {2.52, -0.3};
    NeuralNetwork netAI(numInputs, inputs);

    int numHiddenNeurons = 5;
    std::string activationF = "tanh";
    Layer hiddenLayer(numHiddenNeurons,activationF);
    netAI.addLayer(&hiddenLayer);

    int numOutputNeurons = 3;
    Layer outputLayer(numOutputNeurons);
    netAI.addLayer(&outputLayer);

    netAI.FeedFoward();

    this->net = netAI;
}

void Game::increaseScore()
{
    this->Score += 1;
}

void Game::setScore(int newScore)
{
    this->Score = newScore;
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
