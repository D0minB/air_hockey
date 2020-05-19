#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <circle.h>

class Player
{
private:
    int points_=0;
    Circle *striker_;
    Circle *striker_internal_;
public:
    Player(const int&,const int&, const sf::Color&,const int& up,const int& down);
    ~Player();
    int get_points() const;
    void add_point();
    Circle* get_striker() const;
    Circle* get_striker_internal() const;
    void Player_animate(const sf::Time &elapsed, const bool&);


};


