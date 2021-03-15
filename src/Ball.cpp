#include<chrono>
#include<cmath>
#include<iostream>
#include<random>

#include "Ball.h"

const float PI = 3.1415;

Ball::Ball(sf::Color BallColor):BallColor(BallColor)
{
    restart();
    GameBall.setRadius(config.Radius);
    GameBall.setFillColor(BallColor);
}

Ball::Ball(){}

Ball::~Ball(){}

bool Ball::collideBase(Base BreakoutsBase)
{
    sf::FloatRect BallBounds = GameBall.getGlobalBounds();
    sf::FloatRect BaseBounds = BreakoutsBase.getBaseShape().getGlobalBounds();

    if(BallBounds.intersects(BaseBounds))
    {
        //Upper colliision
        if(GameBall.getPosition().y < BreakoutsBase.getBaseShape().getPosition().y && Vel.y > 0.f)
        {
            float BaseSize = BreakoutsBase.getBaseShape().getSize().x;
            float BaseCenterPosition = BreakoutsBase.getBaseShape().getPosition().x + BaseSize/2;
            float BallCenterPosition = getGameBall().getPosition().x + config.Radius;
            float NewVelAngle = (PI/2) - fabs((PI/3)*(BallCenterPosition - BaseCenterPosition)/(BaseSize/2));

            //Change the ball angle based on the point where it touches the paddle
            sf::Vector2f newVel {config.BallVel * std::cos(NewVelAngle),config.BallVel * std::sin(NewVelAngle)};
            Vel = newVel;
            Vel.y *= -1;

            enum Direction BaseDirection = BreakoutsBase.getDirection();

            if(BaseDirection != Direction::STATIONARY)
            {
                BallDirection = BreakoutsBase.getDirection();
                if(BallDirection == Direction::LEFT)
                    Vel.x *= -1;
            }
            else
            {
                Vel.x = std::abs(Vel.x);
                if(BallDirection == Direction::LEFT)
                    Vel.x *= -1;
            }

        }
        else if(Vel.y > 0) //Side collision
        {
            Vel.x *= -1;
        }

        return true;
    }

    return false;
}

void Ball::update()
{
    //Change direction of the ball
    enum Direction BallDirection = Vel.x > 0 ? Direction::RIGHT : Direction::LEFT;
    setDirection(BallDirection);

    //Move the ball
    GameBall.move(Vel);

    //Check collision with walls
    sf::Vector2f BallPosition = GameBall.getPosition();

    //Left collision
    if(BallPosition.x < 0)
    {
        GameBall.setPosition(0, BallPosition.y);
        float x = (-1)*Vel.x;
        float y =  Vel.y;
        setVel(sf::Vector2f{x, y});
    }

    int WindowSize = config.WindowWidth/2;

    //Right collision
    if(BallPosition.x + 2*config.Radius > WindowSize)
    {
        GameBall.setPosition(WindowSize - 2*config.Radius, BallPosition.y);
        float x = (-1)*Vel.x;
        float y =  Vel.y;
        setVel(sf::Vector2f{x, y});
    }

    //Upper collision
    if(BallPosition.y < 0)
    {
        GameBall.setPosition(BallPosition.x,0);
        float x = Vel.x;
        float y = (-1)*Vel.y;
        setVel(sf::Vector2f{x, y});
    }

    VelAngle = atan2(Vel.x,Vel.y);

    float minAngle = PI * 10 / 180; //CHECK THIS PART
    //Don't let ball angle too small
    if (std::fabs(PI / 2 - std::fabs(VelAngle)) < std::fabs(minAngle))
    {
        float previousVelX = Vel.x;
        float previousVelY = Vel.y;

        Vel = sf::Vector2f(config.BallVel * std::cos(PI / 2 - minAngle), config.BallVel * std::sin(PI / 2 - minAngle));

        if (previousVelX < 0)
            Vel.x *= -1;

        if (previousVelY < 0)
            Vel.y *= -1;
    }
}

void Ball::restart()
{
    GameBall.setPosition(rand()%700 +50,rand()%250 + 450);
    VelAngle = 5*PI/4 + static_cast<float>(rand())/static_cast<float>(RAND_MAX/(PI/2));
    BallDirection = Vel.x > 0 ? Direction::RIGHT : Direction::LEFT;
    Vel.x = config.BallVel * std::cos(VelAngle);
    Vel.y = config.BallVel * std::sin(VelAngle);
}

float Ball::getVelAngle() const
{
    return VelAngle;
}

enum Direction Ball::getDirection()
{
    return BallDirection;
}

sf::Vector2f Ball::getVel()
{
    return Vel;
}

sf::CircleShape& Ball::getGameBall()
{
    return GameBall;
}

void Ball::setVel(sf::Vector2f newVel)
{
    this->Vel = newVel;
}

void Ball::setVelAngle(float VelAngle)
{
    this->VelAngle = VelAngle;
}

void Ball::setDirection(enum Direction Direction)
{
    this->BallDirection = Direction;
}
