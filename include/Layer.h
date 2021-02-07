#ifndef LAYER_H
#define LAYER_H
#include<iostream>
class Layer
{
public:
    Layer(int numNeurons, std::string activationFunction);
    virtual ~Layer();
    int getNumNeurons();
    void linkLayer(Layer* NextLayer); //create weights and biases between two layers
    void insertInputs(double** inputs);
    void calculateOutputs();
    double* getOutputs();
private:
    double* inputs;
    double** weights;
    double* biases;
    double* outputs;
    int numNeurons;
    std::string activationFunction;
    Layer* NextLayer;
};

#endif // LAYER_H
