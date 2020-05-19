#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

class Circle :public sf::CircleShape
{
private:
    int v_x_;
    int v_y_;
    int up_limit_;
    int down_limit_;
public:
    Circle();
    Circle(const int &r, const sf::Vector2f &position,const int &v_x,const int&v_y,const int& up=55,const int& down=785);
    void animate(const sf::Time &elapsed, const bool&);
};

