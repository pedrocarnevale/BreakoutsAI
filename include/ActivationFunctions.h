#ifndef ACTIVATIONFUNCTIONS_H_INCLUDED
#define ACTIVATIONFUNCTIONS_H_INCLUDED
#include<math.h>

double activation(double x, std::string functionName)
{
    if(functionName == "tanh")
        return tanh(x);
    else
        return 0;
}

double tanh(double x)
{
    return tanh(x);
}



#endif // ACTIVATIONFUNCTIONS_H_INCLUDED
