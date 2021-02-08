#include<iostream>
#include<Ball.h>
#include<Game.h>
#include<Base.h>
#include "AuxiliaryFunctions.h"
int main()
{
    //Determine constants
    int WindowWidth = 840;
    int WindowHeight = 900;
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Breakouts Carnevale");
    float radius = 10;
    int FPS = 60;
    int BaseVel = 10;
    int BallVel = 10;
    int NumBlocksLine = 12;
    int NumBlocksColumn = 10;
    int BaseWidth = 100;
    int BaseHeight = 25;
    int BlockMargin = 5;
    int BlockOffset = 50;
    int BlockWidth = WindowWidth/NumBlocksLine - BlockMargin;
    int BlockHeight = 25;
    window.setFramerateLimit(FPS);

    //Construct objects
    Ball ball(&window, radius, BallVel);
    Base base(BaseVel, BaseWidth, BaseHeight, &window);
    Game game(NumBlocksLine, NumBlocksColumn, BlockMargin, BlockOffset, BlockWidth, BlockHeight, &window, &ball, &base);

    //Main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        game.update();
        window.display();
    }

    //To test the neural network
    /*
    int numInputs = 2;
    std::vector<double> inputs= {2.52, -0.3};
    NeuralNetwork netAI(numInputs, inputs);

    int numHiddenNeurons = 3;
    std::string activationF = "tanh";

    Layer hiddenLayer(numHiddenNeurons,activationF);
    netAI.addLayer(&hiddenLayer);


    int numOutputNeurons = 2;
    Layer outputLayer(numOutputNeurons);
    netAI.addLayer(&outputLayer);

    netAI.FeedFoward();

    debugNeuralNetwork(netAI);*/
    return 0;
}
