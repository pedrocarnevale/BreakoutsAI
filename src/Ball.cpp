#include "Ball.h"
#define PI 3.1415
#include<cmath>
#include<iostream>
Ball::Ball(sf::RenderWindow* window, float radius, int BallVel)
{
    this->window = window;
    this->radius = radius;
    this->VelModule = BallVel;
    std::srand(static_cast<unsigned>(time(NULL)));
    restart();
    GameBall.setRadius(radius);
    GameBall.setFillColor(sf::Color::White);
}

Ball::~Ball()
{
    //dtor
}

void Ball::restart()
{
    GameBall.setPosition(rand()%700+50,rand()%250+450);
    VelAngle = 5*PI/4 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(PI/2)));
    direction = Vel.x > 0 ? 1 : 0;
    Vel.x = VelModule*cos(VelAngle);
    Vel.y = VelModule*sin(VelAngle);
}


void Ball::collideBase(Base base)
{
    sf::FloatRect ballBounds = GameBall.getGlobalBounds();
    sf::FloatRect baseBounds = base.getBaseShape().getGlobalBounds();


    if(ballBounds.intersects(baseBounds))
    {
        VelAngle = atan(Vel.y/Vel.x);

        //Upper colliision
        if(GameBall.getPosition().y < base.getBaseShape().getPosition().y && Vel.y>0)
        {
            float baseSize = base.getBaseShape().getSize().x;
            float baseCenterPosition = base.getBaseShape().getPosition().x + baseSize/2;
            float ballCenterPosition = getGameBall().getPosition().x + radius;
            float newVelAngle = (PI/2) - fabs((PI/3)*(ballCenterPosition - baseCenterPosition)/(baseSize/2));

            //Change the ball angle based on the point where it touches the paddle
            sf::Vector2f newVel(VelModule*cos(newVelAngle),VelModule*sin(newVelAngle));
            direction = base.getDirection();
            Vel = newVel;
            Vel.y*=-1;
            if(!direction)
                Vel.x*=-1;

        }

        //Side collision
        else if(Vel.y>0)
        {
            Vel.x*=-1;
        }

    }
}

void Ball::setVel(sf::Vector2f newVel)
{
    this->Vel = newVel;
}

sf::Vector2f Ball::getVel()
{
    return Vel;
}

sf::CircleShape& Ball::getGameBall()
{
    return GameBall;
}

void Ball::setVelAngle(float VelAngle)
{
    this->VelAngle = VelAngle;
}

float Ball::getVelModule()
{
    return VelModule;
}

float Ball::getVelAngle()
{
    return VelAngle;
}

bool Ball::getDirection()
{
    return direction;
}

void Ball::setDirection(bool direction)
{
    this->direction = direction;
}
