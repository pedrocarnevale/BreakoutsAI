#pragma once

#include "NeuralNetwork.h"
#include "Game.h"

void mutation(Game* individual);
void selection(std::vector<Game>& v, int generation);
void crossOver(Game& individual, NeuralNetwork& net1, NeuralNetwork& net2);
void mergeIndividuals(std::vector<Game>& v, int left, int mid, int right);
void mergeSortIndividuals(std::vector<Game>& v, int left, int right);
