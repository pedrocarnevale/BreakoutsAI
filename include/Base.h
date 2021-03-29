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
    Base(sf::Color BaseColor);
    Base();
    ~Base();
    void update(double Left, double Right, double Stationary);
    void restart();
    enum Direction getDirection();
    sf::RectangleShape& getBaseShape();

private:
    sf::Color BaseColor;
    sf::RectangleShape BaseShape;
    enum Direction BaseDirection;
};
