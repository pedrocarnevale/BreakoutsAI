#include<iostream>
#include<random>

#include "Ball.h"
#include "Base.h"
#include "Game.h"

//Define constants
const int BallVel = 10;
const int BaseVel = 10;
const float BaseWidth = 100;
const float BaseHeight = 25;
const int BlockMargin = 5;
const int BlockOffset = 50;
const float BlockHeight = 25;
const int FPS = 60;
const int NumBlocksLine = 12;
const int NumBlocksColumn = 10;
const int NumGames = 1;
const float Radius = 10;
const int WindowWidth = 1700;
const int WindowHeight = 900;
const float BlockWidth = (WindowWidth/2)/NumBlocksLine - BlockMargin;
enum Mode GameType = Mode::NEURAL_NETWORK;
Game* individuals = new Game[NumGames];

int main()
{
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "BreakoutsAI");
    window.setFramerateLimit(FPS);

    //Construct object
    for(int i = 0; i < NumGames; i++)
    {
        Game* newGame = new Game(BallVel, BaseVel, BaseWidth, BaseHeight, BlockMargin,
                                 BlockOffset, BlockWidth, BlockHeight, Radius, NumBlocksLine, NumBlocksColumn, &window);
        individuals[i] = *newGame;
    }

    //Game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for(int i = 0; i < NumGames; i++)
        {
            individuals[i].draw();
            individuals[i].update(GameType);
        }

        window.display();
    }

    delete[] individuals;

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

    debugNeuralNetwork(netAI);
    return 0;
}
