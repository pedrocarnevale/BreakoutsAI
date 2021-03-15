#pragma once

#include "NeuralNetwork.h"
#include "Game.h"

class EvolutionaryAlgorithm
{
    public:
        EvolutionaryAlgorithm();
        void mutation(Game* individual);
        void selection(Game* v, int generation);
        void crossOver(Game& individual, NeuralNetwork& net1, NeuralNetwork& net2);

    private:
        GameConfig config;
};
