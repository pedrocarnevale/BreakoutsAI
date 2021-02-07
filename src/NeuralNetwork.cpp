#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(int numInputs, int* inputs)
{
    this->numLayers = 0;
    this->inputs = new double[numInputs];

    //initialize inputs
    for(int i = 0; i < numInputs; i++)
        this->inputs[i] = inputs[i];
}

NeuralNetwork::~NeuralNetwork()
{
    //dtor
}

void NeuralNetwork::addInputLayer(int NumNeurons, std::string activationFunction)
{
    Layer newLayer(NumNeurons, activationFunction);
    newLayer.insertInputs(&this->inputs);
    this->layers[0] = newLayer;
    numLayers++;
}

void NeuralNetwork::addLayer(Layer* newLayer)
{
    layers[numLayers-1].linkLayer(newLayer);
}

void NeuralNetwork::FeedFoward()
{
    for(int i = 0; i < numLayers - 1; i++)
    {
        double* outputPreviewLayer = layers[i].getOutputs();
        layers[i+1].insertInputs(&outputPreviewLayer);
    }

    this->outputs = layers[numLayers - 1].getOutputs();
}
