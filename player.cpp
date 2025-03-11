#include "player.h"

Player::Player(const int &pos_x, const int &pos_y, const sf::Color &c1, const sf::Color &c2, const int &up_limit, const int &down_limit)
{
    Striker *striker = new Striker(50, sf::Vector2f(pos_x,pos_y), up_limit, down_limit);
    striker_ = striker;
    striker_->setFillColor(c1);
    striker_->setOutlineThickness(3);
    striker_->setOutlineColor(c2);
    striker_->setOrigin(striker->getRadius(),striker->getRadius());

    Striker *striker2 = new Striker(20, sf::Vector2f(pos_x,pos_y), up_limit, down_limit);
    striker_internal_ = striker2;
    striker_internal_->setFillColor(c1);
    striker_internal_->setOutlineThickness(3);
    striker_internal_->setOutlineColor(c2);
    striker_internal_->setOrigin(striker2->getRadius(),striker2->getRadius());
}

Player::~Player()
{
    delete striker_;
    delete striker_internal_;
}

unsigned int Player::get_points() const
{
    return points_;
}

void Player::add_point()
{
    points_++;
}

Striker* Player::get_striker() const
{
    return striker_;
}

Striker* Player::get_striker_internal() const
{
    return striker_internal_;
}

void Player::animate(const sf::Time &elapsed, const bool &arrows, const sf::Vector2f &puck_position, const float &puck_radius)
{
    striker_->animate(elapsed, arrows, puck_position, puck_radius);
    striker_internal_->setPosition(striker_->getPosition().x,striker_->getPosition().y);
}

void Player::reset(const sf::Vector2f &position)
{
    points_ = 0;
    striker_->setPosition(position);
}
