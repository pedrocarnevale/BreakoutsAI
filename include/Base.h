#pragma once

#include<cmath>
#include<ctime>
#include<string>

#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/window.hpp>

#include "utils.h"

class Base
{
public:
    Base(int BaseVel, float BaseWidth, float BaseHeight, sf::RenderWindow* window);

    void update();
    void draw();

    int getBaseVel() const;
    enum Direction getDirection();
    sf::RectangleShape& getBaseShape();

private:
    int BaseVel;
    int BaseWidth;
    int BaseHeight;
    enum Direction BaseDirection;
    sf::RenderWindow* window;
    sf::RectangleShape BaseShape;
};
