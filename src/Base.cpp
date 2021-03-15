#include "Base.h"

Base::Base(sf::Color BaseColor):BaseColor(BaseColor)
{
    BaseShape.setSize(sf::Vector2f{config.BaseWidth,config.BaseHeight});
    BaseShape.setFillColor(BaseColor);

    BaseShape.setPosition((config.WindowWidth/4 - config.BaseWidth/2),config.WindowHeight - 50);

    this->BaseDirection = Direction::STATIONARY;
}

Base::Base()
{
}

Base::~Base()
{

}

void Base::update(double Left, double Stationary, double Right)
{
        if(Left > Right && Left > Stationary)
        {
            Left = 1;
            Right = 0;
        }
        else if(Right > Left && Right > Stationary)
        {
            Left = 0;
            Right = 1;
        }
        else
        {
            Left = 0;
            Right = 0;
        }

    int BasePosition = BaseShape.getPosition().x;

    int WindowSize = config.WindowWidth/2;

    //Move right
    if(Right && (BasePosition + config.BaseWidth < WindowSize))
    {
        BaseShape.move(config.BaseVel,0);
        BaseDirection = Direction::RIGHT;
    }

    //Move left
    else if(Left && BasePosition > 0)
    {
        BaseShape.move(-config.BaseVel,0);
        BaseDirection = Direction::LEFT;
    }

    else
        BaseDirection = Direction::STATIONARY;

    //Check if is out of screen
    if(BasePosition + config.BaseWidth > WindowSize)
        BaseShape.setPosition(WindowSize - config.BaseWidth, config.WindowHeight - 50);
}

void Base::restart()
{
    int WindowSize = static_cast<int>(config.WindowWidth)/2;

    BaseShape.setPosition(WindowSize/2 - config.BaseWidth, config.WindowHeight - 50);
    BaseShape.setSize(sf::Vector2f{config.BaseWidth,config.BaseHeight});
    this->BaseDirection = Direction::STATIONARY;
}

enum Direction Base::getDirection()
{
    return BaseDirection;
}

sf::RectangleShape& Base::getBaseShape()
{
    return BaseShape;
}
