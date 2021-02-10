#include<ctime>

#include "Layer.h"
#include "utils.h"

Layer::Layer(int numNeurons, std::string activationFunction):numNeurons(numNeurons),activationFunction(activationFunction)
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
    this->NextLayer = NextLayer;
    int numNeuronsNextLayer = NextLayer->getNumNeurons();

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
        int numNeuronsNextLayer = NextLayer->getNumNeurons();

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

        this->outputs.resize(numNeuronsNextLayer);

        //activation function
        for(int i = 0; i < numNeuronsNextLayer; i++)
            this->outputs[i] = activation(outputCalculation[i], activationFunction);
    }

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

Layer* Layer::getNextLayer()
{
    return this->NextLayer;
}
