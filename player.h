#pragma once
#include <headers.h>
#include <striker.h>

class Player
{
private:
    int points_=0;
    Striker *striker_;
    Striker *striker_internal_;
public:
    Player(const int&,const int&, const sf::Color&,const int& up,const int& down);
    ~Player();
    int get_points() const;
    void add_point();
    Striker* get_striker() const;
    Striker* get_striker_internal() const;
    void Player_animate(const sf::Time &elapsed, const bool&);


};


