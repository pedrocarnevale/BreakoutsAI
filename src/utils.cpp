#include "utils.h"

float getRandomFloat(float min, float max)
{
    return min + static_cast<float>(rand())/static_cast<float>(RAND_MAX/(max - min));
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
