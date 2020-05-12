#include "player.h"

Player::Player()
{
    this->points=0;
}

int Player::get_points()
{
    return this->points;
}

void Player::add_point()
{
    this->points++;
}
