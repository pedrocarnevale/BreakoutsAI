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
    for(int i = 0;i < Config.NumGames; i++)
        stillAlive[i] = true;

    this->individualsAlive = individuals;
    this->window = window;
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
            individualsAlive[i].draw(Config.GameType);
            individualsAlive[i].update(Config.GameType);
        }

        window->display();
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
