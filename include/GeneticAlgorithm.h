#pragma once

#include<vector>
#include<string>
#include<queue>
#include "Game.h"

class GeneticAlgorithm
{
public:
    GeneticAlgorithm(struct GameConfig Config, sf::RenderWindow* window);
    ~GeneticAlgorithm();

    void update();
    void checkGameOver(int index);
    void checkCollisions(int index);
    void drawMenu();
    void drawBlocks();
    void drawTexts();
    void advanceGeneration();
    std::string updateTime();
    Game* getBestGame();
    Game* getIndividualsAlive();
    std::vector<bool> getStillAlive();

private:
    int Generation;
    int NumIndividuals;
    int record;
    struct GameConfig Config;
    Game* individualsAlive;
    sf::Text textNeuralNetTitle;
    sf::Text textUpper;
    sf::Text textLowerLeft;
    sf::Text textLowerRight;
    std::vector<sf::Text> inputsText;
    std::vector<sf::Text> outputsText;
    std::vector<bool> stillAlive;
    std::vector<std::vector<sf::RectangleShape>> BlocksShape; //Contains the shape to draw on the screen
    std::vector<std::vector<int>> BlocksAvailable; //Contains a bool that indicates if the block is destructed or not
    std::vector<std::vector<sf::FloatRect>> BlocksBounds; //Contains the bounds to check collision with the ball
    sf::Font font;
    sf::Clock clock;
    sf::RenderWindow* window;
};
