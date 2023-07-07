#pragma once
#include <SFML/Graphics.hpp>
#include <striker.h>

enum IsGoal {
    NO_GOAL,
    BLUE_GOAL,
    RED_GOAL
};

class Puck :public sf::CircleShape
{
private:
    sf::Vector2f v_ = sf::Vector2f(-150.f, 0.f);
    const int up_limit_ = 70;
    const int down_limit_ = 935;
    const int left_limit_ = 18;
    const int right_limit_ = 640;
    const sf::Color color_ = sf::Color(255,165,0);

public:
    Puck(const float &r, const sf::Vector2f &position);

    bool animate(const sf::Time &elapsed, std::vector<Striker> &strikers); //PUCK movement
    IsGoal check_goal();
    void reset(const sf::Vector2f &position); //reset PUCK if it is goal
};
