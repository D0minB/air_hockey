#pragma once
#include <headers.h>

class Striker :public sf::CircleShape
{
protected:
    int v_x_;
    int v_y_;
    int up_limit_;
    int down_limit_;
public:
    Striker();
    Striker(const int &r, const sf::Vector2f &position,const int &v_x,const int&v_y,const int& up,const int& down);
    void animate(const sf::Time &elapsed, const bool&);
};

