#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(struct GameConfig Config, sf::RenderWindow* window):Config(Config)
{
    //Construct individuals

    Game* individuals = new Game[Config.NumGames];
    for(int i = 0; i < Config.NumGames; i++)
    {
        Game* newGame = new Game(Config, window);
        individuals[i] = *newGame;
    }

    //initialize matrix
    BlocksShape.resize(Config.NumBlocksLine);
    BlocksAvailable.resize(Config.NumBlocksLine);
    BlocksBounds.resize(Config.NumBlocksLine);

    for(int i = 0; i < Config.NumBlocksLine; i++)
    {
        BlocksShape[i].resize(Config.NumBlocksColumn);
        BlocksAvailable[i].resize(Config.NumBlocksColumn);
        BlocksBounds[i].resize(Config.NumBlocksColumn);
    }

    //Determine pripreties of the blocks
    for(int i = 0; i < Config.NumBlocksLine; i++)
    {
        for(int j = 0; j < Config.NumBlocksColumn; j++)
        {
            sf::RectangleShape Block;
            Block.setSize(sf::Vector2f{Config.BlockWidth,Config.BlockHeight});
            Block.setFillColor(sf::Color(80*(j%4),60*((j+2)%5),127*(j%3),255));
            Block.setPosition((Config.BlockWidth + Config.BlockMargin)*i + Config.BlockMargin, Config.BlockOffset+(Config.BlockHeight + Config.BlockMargin)*j);
            BlocksShape[i][j] = Block;
            BlocksAvailable[i][j] = true;
            BlocksBounds[i][j] = Block.getGlobalBounds();
        }
    }

    stillAlive.resize(Config.NumGames);
    for(int i = 0; i < Config.NumGames; i++)
        stillAlive[i] = true;

    this->individualsAlive = individuals;
    this->window = window;
    this->NumIndividuals = Config.NumGames;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    delete[] individualsAlive;
}

void GeneticAlgorithm::update()
{
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        window->clear();

        for(int i = 0; i < Config.NumGames; i++)
        {
            if(stillAlive[i] == true)
            {
                //Draw individual
                individualsAlive[i].draw();

                //Update
                individualsAlive[i].update();

                //Check if there is a collision
                checkCollisions(i);

                //Check if game was lost
                checkGameOver(i);
            }
        }

        //Draw Menu
        drawMenu();

        //Draw Blocks
        drawBlocks();

        window->display();
    }
}

void GeneticAlgorithm::checkGameOver(int index)
{
    //Check if ball fell down
    Ball* BreakoutsBall = individualsAlive[index].getBreakoutsBall();
    sf::CircleShape BallShape = BreakoutsBall->getGameBall();

    if(BallShape.getPosition().y + 2*BallShape.getRadius() > window->getSize().y)
    {
        individualsAlive[index].setScore(0);
        stillAlive[index] = false;
        NumIndividuals -= 1;

    }
}

void GeneticAlgorithm::checkCollisions(int index)
{
    Ball* BreakoutsBall = individualsAlive[index].getBreakoutsBall();
    sf::FloatRect BallBounds = BreakoutsBall->getGameBall().getGlobalBounds();

    //Check collision with the blocks
    for(int i = 0; i < Config.NumBlocksLine; i++)
    {
        for(int j = 0; j < Config.NumBlocksColumn; j++)
        {
            if(BlocksAvailable[i][j] && BallBounds.intersects(BlocksBounds[i][j]) &&
               BreakoutsBall->getGameBall().getPosition().y < ((Config.BlockHeight + Config.BlockMargin) * Config.NumBlocksColumn) + Config.BlockOffset)
            {
                individualsAlive[i].increaseScore();
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

void GeneticAlgorithm::drawMenu()
{
    //Draw lines
    sf::Vertex line1[] = {sf::Vertex(sf::Vector2f(window->getSize().x/2, 0)), sf::Vertex(sf::Vector2f(window->getSize().x/2, window->getSize().y))};
    sf::Vertex line2[] = {sf::Vertex(sf::Vector2f(window->getSize().x/2, window->getSize().y/2)), sf::Vertex(sf::Vector2f(window->getSize().x, window->getSize().y/2))};

    window->draw(line1, 2, sf::Lines);
    window->draw(line2, 2, sf::Lines);
}

void GeneticAlgorithm::drawBlocks()
{
    for(int i = 0; i < Config.NumBlocksLine; i++)
    {
        for(int j = 0; j < Config.NumBlocksColumn; j++)
        {
            if(BlocksAvailable[i][j])
                window->draw(BlocksShape[i][j]);
        }
    }
}

Game* GeneticAlgorithm::getIndividualsAlive()
{
    return this->individualsAlive;
}

std::vector<bool> GeneticAlgorithm::getStillAlive()
{
    return this->stillAlive;
}
