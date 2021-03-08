#include<iostream>
#include<random>

#include "Ball.h"
#include "Base.h"
#include "Game.h"
#include "GeneticAlgorithm.h"

int main()
{
    struct GameConfig Config;

    sf::RenderWindow window(sf::VideoMode(Config.WindowWidth, Config.WindowHeight), "BreakoutsAI");
    window.setFramerateLimit(Config.FPS);

    //Construct algorithm
    GeneticAlgorithm algorithm(Config, &window);

    //Update algorithm
    algorithm.update();
    /*
    //To test the neural network
    int numInputs = 2;
    std::vector<double> inputs= {2.52, -0.3};
    NeuralNetwork netAI(numInputs, inputs);

    int numHiddenNeurons = 3;
    std::string activationF = "tanh";
    Layer hiddenLayer(numHiddenNeurons,activationF);
    netAI.addLayer(&hiddenLayer);

    int numHiddenNeurons2 = 4;
    std::string activationF2 = "tanh";
    Layer hiddenLayer2(numHiddenNeurons2,activationF2);
    netAI.addLayer(&hiddenLayer2);

    int numOutputNeurons = 2;
    Layer outputLayer(numOutputNeurons);
    netAI.addLayer(&outputLayer);

    netAI.FeedFoward();

    debugNeuralNetwork(netAI);*/
    return 0;
}
