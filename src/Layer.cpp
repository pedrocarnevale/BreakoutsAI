#include "Layer.h"
#include "ActivationFunctions.h"
#include<ctime>
Layer::Layer(int numNeurons, std::string activationFunction)
{
    this->numNeurons = numNeurons;
    this->activationFunction = activationFunction;
}

Layer::~Layer()
{
    //dtor
}

int Layer::getNumNeurons()
{
    return numNeurons;
}

void Layer::linkLayer(Layer* NextLayer)
{
    this->NextLayer = NextLayer;
    int numNeuronsNextLayer = NextLayer->getNumNeurons();

    //Create weights matrix
    weights = new double*[numNeuronsNextLayer];
    for(int i = 0; i < numNeuronsNextLayer; i++)
        weights[i] = new double[numNeurons];

    //Create biases array
    biases = new double[numNeuronsNextLayer];

    //Initialize with random values
    std::srand(static_cast<unsigned>(time(NULL)));
    for(int i = 0; i < numNeuronsNextLayer; i++)
    {
        biases[i] = static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/2)) - 1;
        for(int j = 0; j < numNeurons; j++)
        {
            weights[i][j] = static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/2)) - 1;
        }
    }
}

void Layer::insertInputs(double** inputs)
{
    this->inputs = new double[numNeurons];

    //initialize inputs
    for(int i = 0; i < numNeurons; i++)
        this->inputs[i] = *inputs[i];
}

void Layer::calculateOutputs()
{
    int numNeuronsNextLayer = NextLayer->getNumNeurons();
    this->outputs = new double[numNeuronsNextLayer];

    //initialize outputs
    for(int i = 0; i < numNeuronsNextLayer; i++)
        this->outputs[i] = 0;

    //dot product
    for(int i = 0; i < numNeuronsNextLayer; i++)
    {
        for(int j = 0; j < numNeurons; j++)
        {
            outputs[i] += weights[i][j]*inputs[j];
        }
        //bias
        outputs[i] += biases[i];
    }

    //activation function
    for(int i = 0; i < numNeuronsNextLayer; i++)
        this->outputs[i] = activation(outputs[i], activationFunction);
}

double* Layer::getOutputs()
{
    calculateOutputs();
    return this->outputs;
}
