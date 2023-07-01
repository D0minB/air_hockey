#pragma once
#include "striker.h"

enum IsGoal {
    NO_GOAL,
    BLUE_GOAL,
    RED_GOAL
};

class Puck :public sf::CircleShape
{
private:
    int v_x_ = -150;
    int v_y_ = 0;
    const int up_limit_ = 55;
    const int down_limit_ = 785;
    const int left_limit_ = 18;
    const int right_limit_ = 540;
    const sf::Color color_ = sf::Color(255,140,0);

public:
    Puck(const float &r, const sf::Vector2f &position);

    bool animate(const sf::Time &elapsed, std::vector<Striker> &strikers); //PUCK movement
    IsGoal check_goal();
    void reset(const sf::Vector2f &position); //reset PUCK if is goal
};
