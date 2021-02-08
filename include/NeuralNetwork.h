#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include "Layer.h"
#include<vector>
class NeuralNetwork
{
public:
    NeuralNetwork(int numInputs, std::vector<double> inputs);
    virtual ~NeuralNetwork();
    void addInputLayer(int numNeurons, std::string activationFunction);
    void addLayer(Layer* newLayer);
    void FeedFoward();
    std::vector<double> getInputs();
    std::vector<Layer> getLayers();
private:
    std::vector<double> inputs;
    std::vector<Layer> layers;
    std::vector<double> outputs;
    int numInputs;
};

#endif // NEURALNETWORK_H

