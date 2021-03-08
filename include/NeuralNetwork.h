#pragma once

#include "Layer.h"

class NeuralNetwork
{
public:
    NeuralNetwork(int numInputs, std::vector<double> inputs);
    NeuralNetwork();
    void addInputLayer(int numNeurons, std::string activationFunction);
    void addLayer(Layer* newLayer);
    void FeedFoward();
    void draw();

    std::vector<double> getInputs();
    void setInputs(std::vector<double> newInputs);
    std::vector<Layer> getLayers();
    std::vector<double> getOutputs();
private:
    int numInputs;
    std::vector<double> inputs;
    std::vector<Layer> layers;
    std::vector<double> outputs;
};

