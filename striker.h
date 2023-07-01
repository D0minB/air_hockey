#pragma once
#include "headers.h"

class Striker :public sf::CircleShape
{
protected:
    int v_x_ = 400;
    int v_y_ = 400;
    int up_limit_;
    int down_limit_;
    sf::Vector2f previous_position_;
public:
    Striker();
    Striker(const int &r, const sf::Vector2f &position,const int &v_x,const int&v_y,const int& up,const int& down);
    void animate(const sf::Time &elapsed, const bool&, const sf::Vector2f&);
    void set_previous_position(const sf::Vector2f &p);
    sf::Vector2f get_previous_position();
};

