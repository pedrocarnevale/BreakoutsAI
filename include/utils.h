#pragma once

#include<algorithm>
#include<iostream>
#include<math.h>
#include<vector>

enum class Mode
{
    TRAINING, TESTING
};

enum class Direction
{
    LEFT, RIGHT, STATIONARY
};

namespace GameConfig
{
    const int BallVel = 10;
    const int BaseVel = 10;
    const float BaseWidth = 100;
    const float BaseHeight = 25;
    const int BlockMargin = 5;
    const int BlockOffset = 50;
    const float BlockHeight = 25;
    const int BlockStrength = 3;
    const int CollidedBaseBonus = 0;
    const int CollidedBlockBonus = 1;
    const int DecreaseBaseSizeLimit = INT_MAX;
    const int DeathPenalty = 0.8;
    const float DecreaseFraction = 0.9;
    const int FPS = 1800;
    const float FractionSelection = 0.2;
    const float FractionCrossOver = 0.9;
    const int LayerDistance = 220;
    const int MoveBasePenalty = 0;
    const int NodeDistance = 23;
    const int NumBlocksLine = 12;
    const int NumBlocksColumn = 10;
    const int NumGames = 10;
    const int NumHiddenNeuronsNN = 18;
    const int NumInputsNN = 16;
    const int NumMaxMemory = 5;
    const int NumOutputNeuronsNN = 3;
    const int OffsetXNN = 250;
    const float ProbabilityCrossOver = 1;
    const float ProbabilityMutation = 0.1;
    const float Radius = 10;
    const int RadiusNN = 8;
    const int WindowWidth = 1700;
    const int WindowHeight = 900;
    const float BlockWidth = (WindowWidth/2)/NumBlocksLine - BlockMargin;
};

float getRandomFloat(float min, float max);
double tanh(double x);
double mean(std::vector<double> inputs);
double stdDeviation(std::vector<double> inputs, double mean);
std::vector<double> softmax(std::vector<double> outputs, int numOutputs);
std::vector<double> minMaxScaling(std::vector<double> inputs);
std::vector<double> standardScaler(std::vector<double> inputs);
double activation(double x, std::string functionName);
