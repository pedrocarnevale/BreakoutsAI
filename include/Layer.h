#pragma once

#include<iostream>
#include<vector>

class Layer
{
public:
    Layer(int numNeurons, std::string activationFunction);
    Layer(int numNeurons);
    Layer();

    void linkLayer(Layer* NextLayer); //create weights and biases between two layers
    void insertInputs(std::vector<double> inputs);
    void calculateOutputs();

    int getNumNeurons() const;
    std::vector<double> getOutputs();
    std::vector<std::vector<double>> getWeights();
    std::vector<double> getBiases();
    std::vector<double> getInputs();
    Layer* getNextLayer();

    void setActivationFunction();

private:

    int numNeurons;
    int numNeuronsNextLayer;
    std::vector<double> inputs;
    std::vector<std::vector<double>> weights;
    std::vector<double> biases;
    std::vector<double> outputs;
    std::string activationFunction;
};
