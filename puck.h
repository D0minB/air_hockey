#pragma once
#include "striker.h"

class Puck :public sf::CircleShape
{
private:
    int v_x_ = -150;
    int v_y_ = 0;
    const int up_limit_ = 55;
    const int down_limit_ = 785;
public:
    Puck();
    Puck(const float &r, const sf::Vector2f &position);
    bool animate(const sf::Time &elapsed, std::vector<Striker> &strikers);
    int check_goal();
    void reset(const sf::Vector2f &position); //reset PUCK if is goal
};
