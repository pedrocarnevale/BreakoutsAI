#pragma once

#include<cmath>
#include<ctime>

#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/window.hpp>

#include "Base.h"

class Ball
{
public:
    Ball(sf::Color BallColor);
    Ball();
    ~Ball();
    bool collideBase(Base base);
    void update();
    void restart();

    float getVelAngle() const;
    enum Direction getDirection();
    sf::Vector2f getVel();
    sf::CircleShape& getGameBall();

    void setVel(sf::Vector2f newVel);
    void setVelAngle(float VelAngle);
    void setDirection(enum Direction Direction);

private:
    float VelAngle;
    enum Direction BallDirection;
    sf::Color BallColor;
    sf::CircleShape GameBall;
    sf::Vector2f Vel;
};
