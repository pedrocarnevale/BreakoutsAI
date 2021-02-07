#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include "Layer.h"

class NeuralNetwork
{
public:
    NeuralNetwork(int numInputs, int* inputs);
    virtual ~NeuralNetwork();
    void addInputLayer(int numNeurons, std::string activationFunction);
    void addLayer(Layer* newLayer);
    void FeedFoward();
private:
    double* inputs;
    Layer* layers;
    double* outputs;
    int numLayers;
    int numInputs;
};

#endif // NEURALNETWORK_H

