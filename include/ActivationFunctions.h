#ifndef ACTIVATIONFUNCTIONS_H_INCLUDED
#define ACTIVATIONFUNCTIONS_H_INCLUDED
#include<cmath>

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

double MSE(double x, double realValue)
{
    double diff = (realValue - x);
    return diff*diff/2;
}
#endif // ACTIVATIONFUNCTIONS_H_INCLUDED
