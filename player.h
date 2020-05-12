#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

class Player
{
private:
    int points;
public:
    Player();
    int get_points();
    void add_point();
};


