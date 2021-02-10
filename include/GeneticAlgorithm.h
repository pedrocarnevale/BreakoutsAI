#pragma once

#include<vector>

#include "Game.h"

class GeneticAlgorithm
{
public:
    GeneticAlgorithm(struct GameConfig Config, sf::RenderWindow* window);
    ~GeneticAlgorithm();

    void update();
    void checkGameOver(int i);
    void rankIndividuals(); //Sort individuals using merge sort
    void drawMenu();

    Game* getIndividualsAlive();
    std::vector<bool> getStillAlive();

private:
    int NumIndividuals;
    struct GameConfig Config;
    Game* individualsAlive;
    std::vector<bool> stillAlive;
    sf::RenderWindow* window;
};
