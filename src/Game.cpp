#include<cmath>
#include<iostream>

#include "Game.h"

Game::Game(int BallVel, int BaseVel, float BaseWidth, float BaseHeight, int BlockMargin, int BlockOffset,
            float BlockWidth, float BlockHeight, int Radius, int NumBlocksLine, int NumBlocksColumn, sf::RenderWindow* window)
{
    this->BlockMargin = BlockMargin;
    this->BlockOffset = BlockOffset;
    this->BlockWidth = BlockWidth;
    this->BlockHeight = BlockHeight;
    this->NumBlocksColumn = NumBlocksColumn;
    this->NumBlocksLine = NumBlocksLine;

    Ball* GameBall = new Ball(Radius, BallVel, window);
    this->BreakoutsBall = GameBall;

    Base* GameBase = new Base(BaseVel, BaseWidth, BaseHeight, window);
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
            Block.setPosition((BlockWidth + BlockMargin)*i, BlockOffset+(BlockHeight + BlockMargin)*j);
            BlocksShape[i][j] = Block;
            BlocksAvailable[i][j] = true;
            BlocksBounds[i][j] = Block.getGlobalBounds();
        }
    }
    this->window = window;
}

Game::~Game()
{
    delete this->BreakoutsBall;
    delete this->BreakoutsBase;
}

void Game::updateAndDraw()
{
    //Game loop
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }
        window->clear();

        //Draw
        draw();

        //Update ball
        BreakoutsBall->update(BreakoutsBase);

        //Update base
        BreakoutsBase->update();

        sf::FloatRect BallBounds = BreakoutsBall->getGameBall().getGlobalBounds();

        //Check collision with the blocks
        for(int i = 0; i < NumBlocksLine; i++)
        {
            for(int j = 0; j < NumBlocksColumn; j++)
            {
                if(BlocksAvailable[i][j])
                {
                    if(BallBounds.intersects(BlocksBounds[i][j]))
                    {
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
        }

        //Check if ball fell down
        sf::CircleShape BallShape = BreakoutsBall->getGameBall();
        if(BallShape.getPosition().y + 2*BallShape.getRadius() > window->getSize().y)
            restart();

        window->display();
    }

}

void Game::draw()
{
    //Draw ball
    BreakoutsBall->draw();

    //Draw base
    BreakoutsBase->draw();

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

//Restart the game
void Game::restart()
{
    BreakoutsBall->restart();
    BreakoutsBase->getBaseShape().setPosition((window->getSize().x/2),window->getSize().y - 50);
    for(int i=0; i<NumBlocksLine; i++)
    {
        for(int j=0; j<NumBlocksColumn; j++)
        {
            BlocksAvailable[i][j] = true;
        }
    }
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
