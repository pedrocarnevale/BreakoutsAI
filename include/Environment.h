#pragma once

#include<vector>
#include<string>
#include<queue>
#include "Game.h"
#include "EvolutionaryAlgorithm.h"

class Environment
{
public:
    Environment(sf::RenderWindow* window);

    void update();
    void checkGameOver(int index);
    void checkCollisions(int index);
    void drawGame(Game& individual);
    void drawNeuralNetwork(NeuralNetwork* net);
    void drawLinesShapesNN(std::vector<std::vector<double>>& layerWeights, int numNeuronsPreviousLayer, int numNeuronsNewLayer, int layerIndex);
    void drawMenu();
    void drawTestingText();
    void drawBlocks();
    void drawTexts();
    void drawGraphic();
    void advanceGeneration();
    void updateText(Game& bestPlayer);
    void changeGameMode();
    std::string updateTime();
    Game getBestPlayer();
    Game getBestAveragePlayer();
    std::vector<Game> getIndividualsAlive();

private:
    int Generation;
    int NumIndividuals;
    int record;
    int numTestingDeaths;
    int numTestingWins;
    int numRemainingBlocks;
    float meanVictoryTime;
    float trainingTime;
    std::vector<Game> individualsAlive;
    Mode gameMode;
    sf::RectangleShape button;
    sf::Text textNeuralNetTitle;
    sf::Text textUpper;
    sf::Text textLowerLeft;
    sf::Text textLowerRight;
    std::vector<sf::Text> inputsText;
    std::vector<sf::Text> outputsText;
    std::vector<std::vector<sf::RectangleShape>> BlocksShape; //Contains the shape to draw on the screen
    std::vector<std::vector<int>> BlocksAvailable; //Contains a value that indicates if the block is destructed or not
    std::vector<std::vector<sf::FloatRect>> BlocksBounds; //Contains the bounds to check collision with the ball
    std::vector<double> meanScoreGeneration;
    sf::Font font;
    sf::Clock clock;
    sf::Clock generationClock;
    sf::RenderWindow* window;
};
