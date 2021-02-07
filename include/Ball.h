#ifndef BALL_H
#define BALL_H
#include<Base.h>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<ctime>
#include<cmath>

class Ball
{
public:
    Ball(sf::RenderWindow* window, float radius, int BallVel);
    virtual ~Ball();
    void collideBase(Base base);
    void collideBlock();
    void restart();
    void setVel(sf::Vector2f newVel);
    void setVelAngle(float VelAngle);
    float getVelModule();
    float getVelAngle();
    bool getDirection();
    void setDirection(bool direction);
    sf::Vector2f getVel();
    sf::CircleShape& getGameBall();

private:
    sf::CircleShape GameBall;
    sf::Vector2f Vel;
    float radius;
    float VelAngle;
    float VelModule;
    bool direction; //0 for left - 1 for right
    sf::Vector2f Position;
    sf::RenderWindow* window;
};

#endif // BALL_H
