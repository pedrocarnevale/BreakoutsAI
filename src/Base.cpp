#include "Base.h"

Base::Base(int BaseVel, float BaseWidth, float BaseHeight,  sf::Color BaseColor,  sf::RenderWindow* window):BaseVel(BaseVel),BaseWidth(BaseWidth),BaseHeight(BaseHeight),BaseColor(BaseColor),window(window)
{
    BaseShape.setSize(sf::Vector2f{BaseWidth,BaseHeight});
    BaseShape.setFillColor(BaseColor);
    BaseShape.setPosition((window->getSize().x/4 - BaseWidth/2),window->getSize().y - 50);
    this->BaseDirection = Direction::STATIONARY;
}

void Base::update(double Left, double Right, enum Mode GameType)
{
    if(GameType == Mode::NEURAL_NETWORK)
    {
        if(Left > Right)
        {
            Left = 1;
            Right = 0;
        }
        else
        {
            Left = 0;
            Right = 1;
        }

    }

    int BasePosition = BaseShape.getPosition().x;

    //Move right
    if(Right && (BasePosition + BaseWidth < static_cast <int>(window->getSize().x/2)))
    {
        BaseShape.move(BaseVel,0);
        BaseDirection = Direction::RIGHT;
    }

    //Move left
    else if(Left && BasePosition > 0)
    {
        BaseShape.move(-BaseVel,0);
        BaseDirection = Direction::LEFT;
    }

    else
        BaseDirection = Direction::STATIONARY;

    //Check if is out of screen
    if(BasePosition + BaseWidth > static_cast <int>(window->getSize().x/2))
        BaseShape.setPosition(static_cast <int>(window->getSize().x/2) - BaseWidth, window->getSize().y - 50);
}

void Base::draw()
{
    window->draw(BaseShape);
}

int Base::getBaseVel() const
{
    return BaseVel;
}

int Base::getBaseWidth() const
{
    return BaseVel;
}

int Base::getBaseHeight() const
{
    return BaseVel;
}

enum Direction Base::getDirection()
{
    return BaseDirection;
}

sf::RectangleShape& Base::getBaseShape()
{
    return BaseShape;
}
