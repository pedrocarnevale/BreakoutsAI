#pragma once

#include "NeuralNetwork.h"
#include "Game.h"

class EvolutionaryAlgorithm
{
    public:
        EvolutionaryAlgorithm(GameConfig config, sf::RenderWindow* window);
        void mutation(NeuralNetwork& net);
        void selection(Game* v, int generation);
        NeuralNetwork crossOver(NeuralNetwork& net1, NeuralNetwork& net2);

    private:
        GameConfig config;
        sf::RenderWindow* window;
};
