#pragma once
#include "striker.h"

class Player
{
private:
    unsigned int points_ = 0;
    Striker *striker_;
    Striker *striker_internal_;
public:
    Player(const int &pos_x, const int &pos_y, const sf::Color &c1, const sf::Color &c2, const int &up_limit, const int &down_limit);
    ~Player();

    void add_point();
    void animate(const sf::Time &elapsed, const bool &arrows, const sf::Vector2f &puck_position, const float &puck_radius);
    void reset(const sf::Vector2f &position);

    unsigned int get_points() const;
    Striker* get_striker() const;
    Striker* get_striker_internal() const;
};
