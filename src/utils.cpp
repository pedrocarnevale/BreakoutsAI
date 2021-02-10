#include "utils.h"

float getRandomFloat(float min, float max)
{
    return min + static_cast<float>(rand())/static_cast<float>(RAND_MAX/(max - min));
}

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
    std::vector<double> LayerOutputs = net.getOutputs();
    for(int i = 0; i < numNeurons; i++)
        std::cout<<LayerOutputs[i]<<" ";

}

double mean(std::vector<double> inputs)
{
    double vectorSize = static_cast<double>(inputs.size());
    double sum = 0;
    for(double x: inputs)
        sum += x;
    return sum/vectorSize;
}

double stdDeviation(std::vector<double> inputs, double mean)
{
    double vectorSize = static_cast<double>(inputs.size());
    double sum = 0;
    for(double x: inputs)
        sum += (x - mean) * (x - mean);
    double variance = sum/(vectorSize - 1);
    return sqrt(variance);
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

std::vector<double> minMaxScaling(std::vector<double> inputs)
{
    int InputSize = static_cast<int>(inputs.size());
    std::vector<double> NormalizedVector(InputSize);

    double MaxValue = *std::max_element(inputs.begin(), inputs.end());
    double MinValue = *std::min_element(inputs.begin(), inputs.end());
    double Diff = MaxValue - MinValue;

    for(int i = 0; i < InputSize; i++)
        NormalizedVector[i] = 2*(inputs[i] - MinValue)/Diff - 1;

    return NormalizedVector;
}

std::vector<double> standardScaler(std::vector<double> inputs)
{
    int InputSize = static_cast<int>(inputs.size());
    std::vector<double> NormalizedVector(InputSize);

    double Mean = mean(inputs);
    double StdDeviation = stdDeviation(inputs,Mean);

    for(int i = 0; i < InputSize; i++)
        NormalizedVector[i] = (inputs[i] - Mean)/StdDeviation;

    return NormalizedVector;
}

double activation(double x, std::string functionName)
{
    if(functionName == "tanh")
        return sinh(x)/cosh(x);
    else
    {
        std::cout<<"Activation function error"<<std::endl;
        exit(0);
    }

}
