#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(int numInputs, std::vector<double> inputs):numInputs(numInputs)
{
    this->inputs.resize(numInputs);

    //initialize inputs
    for(int i = 0; i < numInputs; i++)
        this->inputs[i] = inputs[i];
    addInputLayer(numInputs, "tanh");
}

void NeuralNetwork::addInputLayer(int NumNeurons, std::string activationFunction)
{
    Layer newLayer(NumNeurons, activationFunction);
    newLayer.insertInputs(this->inputs);
    this->layers.push_back(newLayer);
}

void NeuralNetwork::addLayer(Layer* newLayer)
{
    layers[layers.size() - 1].linkLayer(newLayer);
    layers.push_back(*newLayer);
}

void NeuralNetwork::FeedFoward()
{
    int numLayers = (int)layers.size();
    for(int i = 0; i < numLayers - 1; i++)
    {
        std::vector<double> outputPreviewLayer = layers[i].getOutputs();
        layers[i+1].insertInputs(outputPreviewLayer);
    }
    this->outputs = layers[numLayers - 1].getOutputs();
}

std::vector<double> NeuralNetwork::getInputs()
{
    return this->inputs;
}

std::vector<Layer> NeuralNetwork::getLayers()
{
    return this->layers;
}
