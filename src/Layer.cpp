#include<ctime>

#include "Layer.h"
#include "utils.h"

Layer::Layer(int numNeurons, std::string activationFunction):numNeurons(numNeurons),numNeuronsNextLayer(0),activationFunction(activationFunction)
{
    this->inputs.resize(numNeurons);
}

Layer::Layer(int numNeurons):numNeurons(numNeurons)
{
    this->activationFunction = "softmax";
}

Layer::Layer()
{
    this->numNeurons = 0;
}

void Layer::linkLayer(Layer* NextLayer)
{
    this->numNeuronsNextLayer = NextLayer->getNumNeurons();

    //Create weights matrix
    weights.resize(numNeuronsNextLayer);
    for(int i = 0; i < numNeuronsNextLayer; i++)
        weights[i].resize(numNeurons);

    //Create biases array
    biases.resize(numNeuronsNextLayer);

    //Initialize with random values
    for(int i = 0; i < numNeuronsNextLayer; i++)
    {
        biases[i] = static_cast <double>(rand()) /( static_cast <double>(RAND_MAX/2)) - 1;
        for(int j = 0; j < numNeurons; j++)
        {
            weights[i][j] = static_cast <double>(rand()) /( static_cast <double>(RAND_MAX/2)) - 1;
        }
    }

    this->outputs.resize(numNeuronsNextLayer);
}

void Layer::insertInputs(std::vector<double> inputs)
{
    this->inputs = inputs;
}

void Layer::calculateOutputs()
{
    if(this->activationFunction == "softmax")
    {
        this->outputs = softmax(this->inputs, numNeurons);
    }
    else
    {
        std::vector<double> outputCalculation(numNeuronsNextLayer,0); //stores the values for the output calculation

        //dot product
        for(int i = 0; i < numNeuronsNextLayer; i++)
        {
            for(int j = 0; j < numNeurons; j++)
            {
                outputCalculation[i] += weights[i][j]*inputs[j];
            }
            //bias
            outputCalculation[i] += biases[i];
        }

        //activation function
        for(int i = 0; i < numNeuronsNextLayer; i++)
            this->outputs[i] = activation(outputCalculation[i], activationFunction);
    }

}

void Layer::setWeights(std::vector<std::vector<double>> newWeights)
{
    this->weights = newWeights;
}

void Layer::setBiases(std::vector<double> newBiases)
{
    this->biases = newBiases;
}

int Layer::getNumNeurons() const
{
    return numNeurons;
}

std::vector<double> Layer::getOutputs()
{
    calculateOutputs();
    return this->outputs;
}

std::vector<std::vector<double>> Layer::getWeights()
{
    return this->weights;
}

std::vector<double> Layer::getBiases()
{
    return this->biases;
}

std::vector<double> Layer::getInputs()
{
    return this->inputs;
}

