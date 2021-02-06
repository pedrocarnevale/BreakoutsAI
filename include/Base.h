#ifndef BASE_H
#define BASE_H

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<ctime>
#include<cmath>
#include<string>

class Base
{
public:
    Base(int BaseVell, int BaseWidth, int BaseHeight, sf::RenderWindow* window);
    virtual ~Base();
    int getBaseVel();
    void update();
    sf::RectangleShape& getBaseShape();
    bool getDirection();
protected:

private:
    int BaseVel;
    int BaseWidth;
    int BaseHeight;
    bool direction; //0 for left - 1 for right
    sf::RenderWindow* window;
    sf::RectangleShape baseShape;
};

#endif // BASE_H
