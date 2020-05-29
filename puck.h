#pragma once
#include <headers.h>
#include <striker.h>

class Puck :public sf::CircleShape
{
private:
    int v_x_=0;
    int v_y_=0;
    int up_limit_=55;
    int down_limit_=785;
public:
    Puck();
    Puck(const float &r, const sf::Vector2f &position,const int &v_x=0,const int&v_y=0);
    bool animate(const sf::Time &elapsed, std::vector<Striker> &strikers);
    void reset_velocity();
    int check_goal();
};
