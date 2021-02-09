#include "utils.h"

void debugNeuralNetwork(NeuralNetwork net)
{
    int numLayers = net.getLayers().size();
    std::cout<<"Welcome to the NeuralNet debugger!"<<std::endl<<std::endl;
    for(int i = 0; i < numLayers - 1; i++)
    {
        int numNeurons = net.getLayers()[i].getNumNeurons();
        int numNeuronsNextLayer = net.getLayers()[i+1].getNumNeurons();

        std::cout<<"Layer "<<i+1<<": "<<std::endl;
        std::cout<<"Inputs: ";
        for(int j = 0; j < numNeurons; j++)
        {
            std::cout<<net.getLayers()[i].getInputs()[j]<<" ";
        }
        std::cout<<std::endl;
        std::cout<<"Weights: ";

        std::vector<std::vector<double>> LayerWeigths = net.getLayers()[i].getWeights();

        for(int i1 = 0; i1 < numNeuronsNextLayer; i1++)
        {
            for(int j1 = 0; j1 < numNeurons; j1++)
            {
                std::cout<<LayerWeigths[i1][j1]<<" ";
            }
        }
        std::cout<<std::endl;

        std::cout<<"Biases: ";
        std::vector<double> LayerBiases = net.getLayers()[i].getBiases();
        for(int j = 0; j < numNeuronsNextLayer; j++)
            std::cout<<LayerBiases[j]<<" ";
        std::cout<<std::endl;

        std::cout<<"Outputs: ";
        std::vector<double> LayerOutputs = net.getLayers()[i].getOutputs();
        for(int j = 0; j < numNeuronsNextLayer; j++)
            std::cout<<LayerOutputs[j]<<" ";

        std::cout<<std::endl<<std::endl;

    }

    int numNeurons = net.getLayers()[numLayers - 1].getNumNeurons();
    std::cout<<"Layer "<<numLayers<<": "<<std::endl;

    std::cout<<"Inputs: ";
    for(int i = 0; i < numNeurons; i++)
    {
        std::cout<<net.getLayers()[numLayers - 1].getInputs()[i]<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"Outputs: ";
    std::vector<double> LayerOutputs = net.getLayers()[numLayers - 1].getOutputs();
    for(int i = 0; i < numNeurons; i++)
        std::cout<<LayerOutputs[i]<<" ";

}

double tanh(double x)
{
    return tanh(x);
}

std::vector<double> softmax(std::vector<double> outputs, int numOutputs)
{
    double sum = 0;
    std::vector<double> result(numOutputs);
    for(int i = 0; i < numOutputs; i++)
    {
        sum += exp(outputs[i]);
    }

    for(int i = 0; i < numOutputs; i++)
        result[i] = exp(outputs[i])/sum;

    return result;
}

double activation(double x, std::string functionName)
{
    if(functionName == "tanh")
        return sinh(x)/cosh(x);
    else
        throw "Activation function error";
}
