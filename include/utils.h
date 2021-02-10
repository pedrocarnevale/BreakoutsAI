#pragma once

#include<algorithm>
#include<iostream>
#include<math.h>
#include<vector>

#include "NeuralNetwork.h"

enum class Direction
{
    LEFT, RIGHT, STATIONARY
};

enum class Mode
{
    KEYBOARD, NEURAL_NETWORK
};

float getRandomFloat(float min, float max);
void debugNeuralNetwork(NeuralNetwork net);
double tanh(double x);
double mean(std::vector<double> inputs);
double stdDeviation(std::vector<double> inputs, double mean);
std::vector<double> softmax(std::vector<double> outputs, int numOutputs);
std::vector<double> minMaxScaling(std::vector<double> inputs);
std::vector<double> standardScaler(std::vector<double> inputs);
double activation(double x, std::string functionName);
