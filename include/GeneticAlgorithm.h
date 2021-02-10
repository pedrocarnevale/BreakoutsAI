#pragma once

#include<vector>

#include "Game.h"

class GeneticAlgorithm
{
public:
    GeneticAlgorithm(struct GameConfig Config, sf::RenderWindow* window);
    ~GeneticAlgorithm();

    void update();
    Game* getIndividualsAlive();
    std::vector<bool> getStillAlive();

private:
    struct GameConfig Config;
    Game* individualsAlive;
    std::vector<bool> stillAlive;
    sf::RenderWindow* window;
};
