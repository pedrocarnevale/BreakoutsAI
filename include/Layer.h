#ifndef LAYER_H
#define LAYER_H
#include<iostream>
#include<vector>
class Layer
{
public:
    Layer(int numNeurons, std::string activationFunction);
    Layer(int numNeurons);
    Layer();
    virtual ~Layer();
    int getNumNeurons();
    void setNumNeurons();
    void setActivationFunction();
    void linkLayer(Layer* NextLayer); //create weights and biases between two layers
    void insertInputs(std::vector<double> inputs);
    void calculateOutputs();
    std::vector<double> getOutputs();
    std::vector<std::vector<double>> getWeights();
    std::vector<double> getBiases();
    std::vector<double> getInputs();
    Layer* getNextLayer();
private:
    std::vector<double> inputs;
    std::vector<std::vector<double>> weights;
    std::vector<double> biases;
    std::vector<double> outputs;
    int numNeurons;
    std::string activationFunction;
    Layer* NextLayer;
};

#endif // LAYER_H
