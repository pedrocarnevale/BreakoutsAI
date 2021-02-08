#include "Game.h"
#include<iostream>
#include<cmath>

Game::Game(int NumBlocksLine,int NumBlocksColumn, int BlockMargin, int BlockOffset, int BlockWidth, int BlockHeight, sf::RenderWindow* window, Ball* ball, Base* base)
{
    this->BlockMargin = BlockMargin;
    this->BlockOffset = BlockOffset;
    this->BlockWidth = BlockWidth;
    this->BlockHeight = BlockHeight;
    this->NumBlocksColumn = NumBlocksColumn;
    this->NumBlocksLine = NumBlocksLine;
    this->ball = ball;
    this->base = base;

    //initialize matrix
    BlocksShape.resize(NumBlocksLine);
    BlocksAvailable.resize(NumBlocksLine);
    BlocksBounds.resize(NumBlocksLine);

    for(int i=0; i<NumBlocksLine;i++)
    {
        BlocksShape[i].resize(NumBlocksColumn);
        BlocksAvailable[i].resize(NumBlocksColumn);
        BlocksBounds[i].resize(NumBlocksColumn);
    }
    //Determine pripreties of the blocks
    for(int i=0; i<NumBlocksLine; i++)
    {
        for(int j=0; j<NumBlocksColumn; j++)
        {
            sf::RectangleShape block;
            block.setSize(sf::Vector2f(BlockWidth,BlockHeight));
            block.setFillColor(sf::Color(80*(j%4),60*((j+2)%5),127*(j%3),255));
            block.setPosition((BlockWidth + BlockMargin)*i, BlockOffset+(BlockHeight + BlockMargin)*j);
            BlocksShape[i][j] = block;
            BlocksAvailable[i][j] = true;
            BlocksBounds[i][j] = block.getGlobalBounds();
        }
    }
    this->window = window;
}

Game::~Game()
{
    //dtor
}

int Game::getNumBlocksLine()
{
    return NumBlocksLine;
}
int Game::getNumBlocksColumn()
{
    return NumBlocksColumn;
}
int Game::getBlockMargin()
{
    return BlockMargin;
}
int Game::getBlockOffset()
{
    return BlockOffset;
}
int Game::getBlockWidth()
{
    return BlockWidth;
}
int Game::getBlockHeight()
{
    return BlockHeight;
}

void Game::update()
{
    //Update ball
    updateBall();
    //Update base
    base->update();

    sf::FloatRect ballBounds = ball->getGameBall().getGlobalBounds();

    //Draw blocks and check collision with the blocks
    for(int i=0; i<NumBlocksLine; i++)
    {
        for(int j=0; j<NumBlocksColumn; j++)
        {
            if(BlocksAvailable[i][j])
            {
                window->draw(BlocksShape[i][j]);
                if(ballBounds.intersects(BlocksBounds[i][j]))
                {
                    BlocksAvailable[i][j] = false;
                    //Upper collision
                    float upperDistance = abs(BlocksBounds[i][j].top - (ballBounds.top + ballBounds.height));

                    //Bottom collision
                    float bottomDistance = abs((BlocksBounds[i][j].top + BlocksBounds[i][j].height) - ballBounds.top);

                    //Right collision
                    float rightDistance = abs((BlocksBounds[i][j].left + BlocksBounds[i][j].width) - ballBounds.left);

                    //Left collision
                    float leftDistance = abs(BlocksBounds[i][j].left - (ballBounds.left + ballBounds.width));

                    //Determine which distance is the smallest to check the type of collision
                    float minDistance = std::min(std::min(upperDistance, bottomDistance), std::min(rightDistance, leftDistance));

                    sf::Vector2f newVel;

                    //Vertical collision
                    if(minDistance == upperDistance || minDistance == bottomDistance)
                    {
                        newVel.x = ball->getVel().x;
                        newVel.y = (-1)*ball->getVel().y;
                        ball->setVel(newVel);
                    }

                    //Horizontal collision
                    else if(minDistance == leftDistance || minDistance == rightDistance)
                    {
                        newVel.x = (-1)*ball->getVel().x;
                        newVel.y =  ball->getVel().y;
                        ball->setVel(newVel);
                    }
                }
            }
        }
    }
}

void Game::updateBall()
{
    //Check if there is collision with the base
    ball->collideBase(*base);

    //Change direction of the ball
    bool ballDirection = ball->getVel().x > 0 ? 1 : 0;
    ball->setDirection(ballDirection);

    //Move the ball
    sf::CircleShape* ballShape = &ball->getGameBall();
    sf::Vector2f ballVel = ball->getVel();
    window->draw(*ballShape);
    ballShape->move(ballVel);

    //Check collision with walls

    //Left collision
    if(ballShape->getPosition().x<0)
    {
        ballShape->setPosition(0,ballShape->getPosition().y);
        sf::Vector2f newVel;
        newVel.x = (-1)*ball->getVel().x;
        newVel.y =  ball->getVel().y;
        ball->setVel(newVel);
    }

    //Right collision
    if(ballShape->getPosition().x + 2*ballShape->getRadius()>window->getSize().x)
    {
        ballShape->setPosition(window->getSize().x - 2*ballShape->getRadius(),ballShape->getPosition().y);
        sf::Vector2f newVel;
        newVel.x = (-1)*ball->getVel().x;
        newVel.y =  ball->getVel().y;
        ball->setVel(newVel);
    }

    //Upper collision
    if(ballShape->getPosition().y<0)
    {
        ballShape->setPosition(ballShape->getPosition().x,0);
        sf::Vector2f newVel;
        newVel.x = ball->getVel().x;
        newVel.y = (-1)*ball->getVel().y;
        ball->setVel(newVel);
    }

    //Ball fell down
    if(ballShape->getPosition().y + 2*ballShape->getRadius()>window->getSize().y)
    {
        restart();
    }
}

//Restart the game
void Game::restart()
{
    ball->restart();
    base->getBaseShape().setPosition((window->getSize().x/2),window->getSize().y - 50);
    for(int i=0; i<NumBlocksLine; i++)
    {
        for(int j=0; j<NumBlocksColumn; j++)
        {
            BlocksAvailable[i][j] = true;
        }
    }
}

void Game::addNeuralNetwork(NeuralNetwork* net)
{
    this->net = net;
}
