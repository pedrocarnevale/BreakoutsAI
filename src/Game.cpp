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
    Blocks = new std::pair<sf::RectangleShape,int> *[NumBlocksLine];
    BlocksBounds = new sf::FloatRect *[NumBlocksLine];
    for(int i=0; i<NumBlocksLine; i++)
    {
        Blocks[i] = new std::pair<sf::RectangleShape,int>[NumBlocksColumn];
        BlocksBounds[i] = new sf::FloatRect[NumBlocksColumn];
    }

    for(int i=0; i<NumBlocksLine; i++)
    {
        for(int j=0; j<NumBlocksColumn; j++)
        {
            sf::RectangleShape block;
            block.setSize(sf::Vector2f(BlockWidth,BlockHeight));
            block.setFillColor(sf::Color(80*(j%4),60*((j+2)%5),127*(j%3),255));
            block.setPosition((BlockWidth + BlockMargin)*i, BlockOffset+(BlockHeight + BlockMargin)*j);
            Blocks[i][j].first = block;
            Blocks[i][j].second = true;
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
    //Area of the blocks
    sf::FloatRect ballBounds = ball->getGameBall().getGlobalBounds();
    for(int i=0; i<NumBlocksLine; i++)
    {
        for(int j=0; j<NumBlocksColumn; j++)
        {
            if(Blocks[i][j].second)
            {
                window->draw(Blocks[i][j].first);
                if(ballBounds.intersects(BlocksBounds[i][j]))
                {
                    Blocks[i][j].second = false;
                    //Upper collision
                    float upperDistance = abs(BlocksBounds[i][j].top - (ballBounds.top + ballBounds.height));

                    //Bottom collision
                    float bottomDistance = abs((BlocksBounds[i][j].top + BlocksBounds[i][j].height) - ballBounds.top);

                    //Right collision
                    float rightDistance = abs((BlocksBounds[i][j].left + BlocksBounds[i][j].width) - ballBounds.left);

                    //Left collision
                    float leftDistance = abs(BlocksBounds[i][j].left - (ballBounds.left + ballBounds.width));

                    float minDistance = std::min(std::min(upperDistance, bottomDistance), std::min(rightDistance, leftDistance));

                    sf::Vector2f newVel;

                    if(minDistance == upperDistance || minDistance == bottomDistance)
                    {
                        newVel.x = ball->getVel().x;
                        newVel.y = (-1)*ball->getVel().y;
                        ball->setVel(newVel);
                    }

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
    ball->collideBase(*base);
    bool ballDirection = ball->getVel().x > 0 ? 1 : 0;
    ball->setDirection(ballDirection);
    sf::CircleShape* ballShape = &ball->getGameBall();
    sf::Vector2f ballVel = ball->getVel();
    window->draw(*ballShape);
    ballShape->move(ballVel);
    if(ballShape->getPosition().x<0)
    {
        ballShape->setPosition(0,ballShape->getPosition().y);
        sf::Vector2f newVel;
        newVel.x = (-1)*ball->getVel().x;
        newVel.y =  ball->getVel().y;
        ball->setVel(newVel);
    }

    if(ballShape->getPosition().x + 2*ballShape->getRadius()>window->getSize().x)
    {
        ballShape->setPosition(window->getSize().x - 2*ballShape->getRadius(),ballShape->getPosition().y);
        sf::Vector2f newVel;
        newVel.x = (-1)*ball->getVel().x;
        newVel.y =  ball->getVel().y;
        ball->setVel(newVel);
    }

    if(ballShape->getPosition().y<0)
    {
        ballShape->setPosition(ballShape->getPosition().x,0);
        sf::Vector2f newVel;
        newVel.x = ball->getVel().x;
        newVel.y = (-1)*ball->getVel().y;
        ball->setVel(newVel);
    }

    if(ballShape->getPosition().y + 2*ballShape->getRadius()>window->getSize().y)
    {
        restart();
    }
}

void Game::restart()
{
    ball->restart();
    base->getBaseShape().setPosition((window->getSize().x/2),window->getSize().y - 50);
    for(int i=0; i<NumBlocksLine; i++)
    {
        for(int j=0; j<NumBlocksColumn; j++)
        {
            Blocks[i][j].second = true;
        }
    }
}
