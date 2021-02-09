#include "Base.h"

Base::Base(int BaseVel, float BaseWidth, float BaseHeight, sf::RenderWindow* window):BaseVel(BaseVel),BaseWidth(BaseWidth),BaseHeight(BaseHeight),window(window)
{
    BaseShape.setSize(sf::Vector2f{BaseWidth,BaseHeight});
    BaseShape.setFillColor(sf::Color::White);
    BaseShape.setPosition((window->getSize().x/2),window->getSize().y - 50);
    this->BaseDirection = Direction::STATIONARY;
}

void Base::update()
{
    bool PressedRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    bool PressedLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    int BasePosition = BaseShape.getPosition().x;

    //Move right
    if(PressedRight && (BasePosition + BaseWidth < static_cast <int>(window->getSize().x)))
    {
        BaseShape.move(BaseVel,0);
        BaseDirection = Direction::RIGHT;
    }

    //Move left
    else if(PressedLeft && BasePosition > 0)
    {
        BaseShape.move(-BaseVel,0);
        BaseDirection = Direction::LEFT;
    }

    else
        BaseDirection = Direction::STATIONARY;
}

void Base::draw()
{
    window->draw(BaseShape);
}

int Base::getBaseVel() const
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
