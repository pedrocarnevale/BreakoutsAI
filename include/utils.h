#pragma once

#include<algorithm>
#include<iostream>
#include<math.h>
#include<vector>

enum class Direction
{
    LEFT, RIGHT, STATIONARY
};

struct GameConfig
{
    static const int BallVel = 10;
    static const int BaseVel = 10;
    static constexpr float BaseWidth = 100;
    static constexpr float BaseHeight = 25;
    static const int BlockMargin = 5;
    static const int BlockOffset = 50;
    static constexpr float BlockHeight = 25;
    static const int BlockStrength = 3;
    static const int FPS = 60;
    static constexpr float FractionSelection = 0.2;
    static constexpr float ProbabilityCrossOver = 0.6;
    static constexpr float ProbabilityMutation = 0.1;
    static const int LayerDistance = 220;
    static const int NodeDistance = 80;
    static const int NumBlocksLine = 12;
    static const int NumBlocksColumn = 10;
    static const int NumGames = 20;
    static const int NumHiddenNeuronsNN = 5;
    static const int NumInputsNN = 3;
    static const int NumOutputNeuronsNN = 3;
    static const int OffsetXNN = 200;
    static constexpr float Radius = 10;
    static const int RadiusNN = 25;
    static const int WindowWidth = 1700;
    static const int WindowHeight = 900;
    static constexpr float BlockWidth = (WindowWidth/2)/NumBlocksLine - BlockMargin;
};

float getRandomFloat(float min, float max);
double tanh(double x);
double mean(std::vector<double> inputs);
double stdDeviation(std::vector<double> inputs, double mean);
std::vector<double> softmax(std::vector<double> outputs, int numOutputs);
std::vector<double> minMaxScaling(std::vector<double> inputs);
std::vector<double> standardScaler(std::vector<double> inputs);
double activation(double x, std::string functionName);
