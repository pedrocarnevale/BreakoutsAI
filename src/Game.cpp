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

    Base* GameBase = new Base(Config.BaseVel, Config.BaseWidth, Config.BaseHeight, Config.GameType, GameColor, window);
    this->BreakoutsBase = GameBase;

    //initialize matrix
    BlocksShape.resize(NumBlocksLine);
    BlocksAvailable.resize(NumBlocksLine);
    BlocksBounds.resize(NumBlocksLine);

    for(int i = 0; i < NumBlocksLine; i++)
    {
        BlocksShape[i].resize(NumBlocksColumn);
        BlocksAvailable[i].resize(NumBlocksColumn);
        BlocksBounds[i].resize(NumBlocksColumn);
    }
    //Determine pripreties of the blocks
    for(int i = 0; i < NumBlocksLine; i++)
    {
        for(int j = 0; j < NumBlocksColumn; j++)
        {
            sf::RectangleShape Block;
            Block.setSize(sf::Vector2f{BlockWidth,BlockHeight});
            Block.setFillColor(sf::Color(80*(j%4),60*((j+2)%5),127*(j%3),255));
            Block.setPosition((BlockWidth + BlockMargin)*i + BlockMargin, BlockOffset+(BlockHeight + BlockMargin)*j);
            BlocksShape[i][j] = Block;
            BlocksAvailable[i][j] = true;
            BlocksBounds[i][j] = Block.getGlobalBounds();
        }
    }
    this->window = window;
}

Game::Game()
{

}

Game::~Game()
{
    delete this->BreakoutsBall;
    delete this->BreakoutsBase;
}

void Game::update(enum Mode GameType)
{
    //Update ball
    BreakoutsBall->update(BreakoutsBase, GameType);

    //Update base using Keyboard
    if(GameType == Mode::KEYBOARD)
    {
        double PressedLeft = static_cast<double>(sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
        double PressedRight = static_cast<double>(sf::Keyboard::isKeyPressed(sf::Keyboard::Right));

        BreakoutsBase->update(PressedLeft, PressedRight, GameType);
    }

    //Update base using Neural Network
    if(GameType == Mode::NEURAL_NETWORK)
    {
        sf::CircleShape GameBall = BreakoutsBall->getGameBall();
        sf::RectangleShape GameBase = BreakoutsBase->getBaseShape();
        sf::Vector2f BallVel = BreakoutsBall->getVel();

        double BallPositionX = GameBall.getPosition().x;
        double BallPositionY = GameBall.getPosition().y;
        double BallVelX = BallVel.x;
        double BallVelY = BallVel.y;
        double BasePosition = GameBase.getPosition().x;

        std::vector<double> inputs = {BallPositionX, BallPositionY, BallVelX, BallVelY, BasePosition};
        inputs = standardScaler(inputs);
        int numInputs = 5;
        NeuralNetwork Brain(numInputs, inputs);

        int numHiddenNeurons = 3;
        std::string activationF = "tanh";
        Layer hiddenLayer(numHiddenNeurons,activationF);
        Brain.addLayer(&hiddenLayer);

        int numOutputNeurons = 2;
        Layer outputLayer(numOutputNeurons);
        Brain.addLayer(&outputLayer);

        Brain.FeedFoward();

        std::vector<double> NetOutputs = Brain.getOutputs();
        double Left = NetOutputs[0];
        double Right = NetOutputs[1];

        BreakoutsBase->update(Left, Right, GameType);
    }

    sf::FloatRect BallBounds = BreakoutsBall->getGameBall().getGlobalBounds();

    //Check collision with the blocks
    for(int i = 0; i < NumBlocksLine; i++)
    {
        for(int j = 0; j < NumBlocksColumn; j++)
        {
            if(BlocksAvailable[i][j] && BallBounds.intersects(BlocksBounds[i][j]) && BreakoutsBall->getGameBall().getPosition().y < ((BlockHeight + BlockMargin) * NumBlocksColumn) + BlockOffset)
            {
                Score+=1;
                BlocksAvailable[i][j] = false;
                //Upper collision
                float upperDistance = std::abs(BlocksBounds[i][j].top - (BallBounds.top + BallBounds.height));

                //Bottom collision
                float bottomDistance = std::abs((BlocksBounds[i][j].top + BlocksBounds[i][j].height) - BallBounds.top);

                //Right collision
                float rightDistance = std::abs((BlocksBounds[i][j].left + BlocksBounds[i][j].width) - BallBounds.left);

                //Left collision
                float leftDistance = std::abs(BlocksBounds[i][j].left - (BallBounds.left + BallBounds.width));

                //Determine which distance is the smallest to check the type of collision
                float minDistance = std::min(std::min(upperDistance, bottomDistance), std::min(rightDistance, leftDistance));

                //Vertical collision
                if(minDistance == upperDistance || minDistance == bottomDistance)
                {
                    float x = BreakoutsBall->getVel().x;
                    float y = (-1)*BreakoutsBall->getVel().y;
                    BreakoutsBall->setVel(sf::Vector2f{x, y});
                }

                //Horizontal collision
                else if(minDistance == leftDistance || minDistance == rightDistance)
                {
                    float x = (-1)*BreakoutsBall->getVel().x;
                    float y =  BreakoutsBall->getVel().y;
                    BreakoutsBall->setVel(sf::Vector2f{x, y});

                }
            }
        }
    }

    //Check if ball fell down
    sf::CircleShape BallShape = BreakoutsBall->getGameBall();
    if(BallShape.getPosition().y + 2*BallShape.getRadius() > window->getSize().y)
    {
        BreakoutsBall->restart();
        BreakoutsBase->restart(GameType);
        for(int i=0; i<NumBlocksLine; i++)
        {
            for(int j=0; j<NumBlocksColumn; j++)
            {
                BlocksAvailable[i][j] = true;
            }
        }
    }
}

void Game::draw(enum Mode GameType)
{
    //Draw ball
    BreakoutsBall->draw();

    //Draw base
    BreakoutsBase->draw();

    if(GameType == Mode::NEURAL_NETWORK)
    {
        //Draw lines
        sf::Vertex line1[] = {sf::Vertex(sf::Vector2f(window->getSize().x/2, 0)), sf::Vertex(sf::Vector2f(window->getSize().x/2, window->getSize().y))};
        sf::Vertex line2[] = {sf::Vertex(sf::Vector2f(window->getSize().x/2, window->getSize().y/2)), sf::Vertex(sf::Vector2f(window->getSize().x, window->getSize().y/2))};

        window->draw(line1, 2, sf::Lines);
        window->draw(line2, 2, sf::Lines);
    }

    //Draw blocks
    for(int i = 0; i < NumBlocksLine; i++)
    {
        for(int j = 0; j < NumBlocksColumn; j++)
        {
            if(BlocksAvailable[i][j])
            {
                window->draw(BlocksShape[i][j]);
            }
        }
    }

}

void Game::addNeuralNetwork(NeuralNetwork* net)
{
    this->net = net;
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
