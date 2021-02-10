#pragma once

#include "Layer.h"

class NeuralNetwork
{
public:
    NeuralNetwork(int numInputs, std::vector<double> inputs);

    void addInputLayer(int numNeurons, std::string activationFunction);
    void addLayer(Layer* newLayer);
    void FeedFoward();

    std::vector<double> getInputs();
    std::vector<Layer> getLayers();
    std::vector<double> getOutputs();
private:
    int numInputs;
    std::vector<double> inputs;
    std::vector<Layer> layers;
    std::vector<double> outputs;
};

