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
                individualsAlive[i].draw(Config.GameType);

                //Update
                individualsAlive[i].update(Config.GameType);

                //Check if game was lost
                checkGameOver(i);
                std::cout<<NumIndividuals<<std::endl;
            }
        }

        //Draw Menu
        if(Config.GameType == Mode::NEURAL_NETWORK)
                    drawMenu();

        window->display();
    }
}

void GeneticAlgorithm::checkGameOver(int i)
{
    //Check if ball fell down
    Ball* BreakoutsBall = individualsAlive[i].getBreakoutsBall();
    sf::CircleShape BallShape = BreakoutsBall->getGameBall();

    if(BallShape.getPosition().y + 2*BallShape.getRadius() > window->getSize().y)
    {
        if(Config.GameType == Mode::KEYBOARD)
            individualsAlive[i].restart(Config.GameType);
        if(Config.GameType == Mode::NEURAL_NETWORK)
        {
            stillAlive[i] = false;
            NumIndividuals -= 1;
        }

    }
}

void GeneticAlgorithm::rankIndividuals()
{

}

void GeneticAlgorithm::drawMenu()
{
    //Draw lines
    sf::Vertex line1[] = {sf::Vertex(sf::Vector2f(window->getSize().x/2, 0)), sf::Vertex(sf::Vector2f(window->getSize().x/2, window->getSize().y))};
    sf::Vertex line2[] = {sf::Vertex(sf::Vector2f(window->getSize().x/2, window->getSize().y/2)), sf::Vertex(sf::Vector2f(window->getSize().x, window->getSize().y/2))};

    window->draw(line1, 2, sf::Lines);
    window->draw(line2, 2, sf::Lines);
}

Game* GeneticAlgorithm::getIndividualsAlive()
{
    return this->individualsAlive;
}

std::vector<bool> GeneticAlgorithm::getStillAlive()
{
    return this->stillAlive;
}
