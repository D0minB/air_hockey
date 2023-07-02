#pragma once
#include "striker.h"

class Player
{
private:
    int points_ = 0;
    Striker *striker_;
    Striker *striker_internal_;
public:
    Player(const int&, const int&, const sf::Color&, const int& up, const int& down);
    ~Player();

    void add_point();
    void animate(const sf::Time &elapsed, const bool&, const sf::Vector2f&);
    void reset(const sf::Vector2f &position);

    int get_points() const;
    Striker* get_striker() const;
    Striker* get_striker_internal() const;
};


