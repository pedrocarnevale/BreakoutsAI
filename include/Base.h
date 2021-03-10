#pragma once

#include<chrono>
#include<cmath>
#include<ctime>
#include<string>
#include<random>

#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/window.hpp>

#include "NeuralNetwork.h"
#include "utils.h"

class Base
{
public:
    Base(int BaseVel, float BaseWidth, float BaseHeight, sf::Color BaseColor, sf::RenderWindow* window);
    Base();
    ~Base();
    void update(double Left, double Right, double Stationary);
    void draw();
    void restart();
    int getBaseVel() const;
    int getBaseWidth() const;
    int getBaseHeight() const;
    enum Direction getDirection();
    sf::RectangleShape& getBaseShape();

private:
    int BaseVel;
    int BaseWidth;
    int BaseHeight;
    sf::Color BaseColor;
    sf::RenderWindow* window;
    sf::RectangleShape BaseShape;
    enum Direction BaseDirection;
};
