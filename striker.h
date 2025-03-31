#pragma once
#include "headers.h"

class Striker :public sf::CircleShape
{
protected:
    sf::Vector2f v_ = sf::Vector2f(300.f, 300.f);
    int up_limit_;
    int down_limit_;
    int left_limit_ = 30;
    int right_limit_ = 630;
    sf::Vector2f previous_position_;

public:
    Striker(const int &r, const sf::Vector2f &position, const int& up, const int& down);

    void animate(const sf::Time &elapsed, const bool&, const sf::Vector2f&, const float&);
    void set_previous_position(const sf::Vector2f &p);
    sf::Vector2f get_previous_position();
};

