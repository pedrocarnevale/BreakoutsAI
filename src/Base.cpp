#include "Base.h"

Base::Base(int BaseVel, int BaseWidth, int BaseHeight, sf::RenderWindow* window)
{
    this->BaseVel = BaseVel;
    this->BaseWidth = BaseWidth;
    this->BaseHeight = BaseHeight;
    baseShape.setSize(sf::Vector2f(BaseWidth,BaseHeight));
    baseShape.setFillColor(sf::Color::White);
    baseShape.setPosition((window->getSize().x/2),window->getSize().y - 50);
    this->window = window;
}

Base::~Base()
{
    //dtor
}

int Base::getBaseVel()
{
    return BaseVel;
}

void Base::update()
{
    bool PressedRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    bool PressedLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    int basePosition = baseShape.getPosition().x;

    //Move right
    if(PressedRight && (basePosition + BaseWidth < (int)window->getSize().x))
    {
        baseShape.move(BaseVel,0);
        direction = 1;
    }


    //Move left
    if(PressedLeft && basePosition > 0)
    {
        baseShape.move(-BaseVel,0);
        direction = 0;
    }
    window->draw(baseShape);
}

sf::RectangleShape& Base::getBaseShape()
{
    return baseShape;
}

bool Base::getDirection()
{
    return direction;
}
