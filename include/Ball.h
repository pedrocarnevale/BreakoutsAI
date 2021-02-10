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
    Ball(float Radius, int BallVel, sf::Color BallColor, sf::RenderWindow* window);

    void collideBase(Base base);
    void update(Base* base, enum Mode GameType);
    void draw();
    void restart();

    float getVelModule() const;
    float getVelAngle() const;
    enum Direction getDirection();
    sf::Vector2f getVel();
    sf::CircleShape& getGameBall();

    void setVel(sf::Vector2f newVel);
    void setVelAngle(float VelAngle);
    void setDirection(enum Direction Direction);

private:
    float Radius;
    float VelAngle;
    float VelModule;
    enum Direction BallDirection;
    sf::Color BallColor;
    sf::RenderWindow* window;
    sf::CircleShape GameBall;
    sf::Vector2f Vel;
};
