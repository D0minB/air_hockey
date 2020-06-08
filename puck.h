#pragma once
#include "headers.h"
#include "striker.h"

class Puck :public sf::CircleShape
{
private:
    int v_x_=-150;
    int v_y_=0;
    int up_limit_=55;
    int down_limit_=785;
public:
    Puck();
    Puck(const float &r, const sf::Vector2f &position);
    bool animate(const sf::Time &elapsed, std::vector<Striker> &strikers);
    void reset_velocity();
    int check_goal();
    void reset(const sf::Vector2f &position);
};
