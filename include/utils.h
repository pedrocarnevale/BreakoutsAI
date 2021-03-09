#pragma once

#include<algorithm>
#include<iostream>
#include<math.h>
#include<vector>

#include "NeuralNetwork.h"

enum class Mode
{
    KEYBOARD, NEURAL_NETWORK
};

enum class Direction
{
    LEFT, RIGHT, STATIONARY
};

struct GameConfig
{
    const int BallVel = 10;
    const int BaseVel = 10;
    const float BaseWidth = 100;
    const float BaseHeight = 25;
    const int BlockMargin = 5;
    const int BlockOffset = 50;
    const float BlockHeight = 25;
    const int BlockStrength = 3;
    const int FPS = 60;
    const int NumBlocksLine = 12;
    const int NumBlocksColumn = 10;
    const int NumGames = 100;
    const float Radius = 10;
    const int WindowWidth = 1700;
    const int WindowHeight = 900;
    const float BlockWidth = (WindowWidth/2)/NumBlocksLine - BlockMargin;
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

