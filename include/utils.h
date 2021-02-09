#pragma once

#include<iostream>
#include<math.h>
#include<vector>

#include "NeuralNetwork.h"

enum class Direction
{
    LEFT, RIGHT, STATIONARY
};

void debugNeuralNetwork(NeuralNetwork net);
double tanh(double x);
std::vector<double> softmax(std::vector<double> outputs, int numOutputs);
double activation(double x, std::string functionName);
