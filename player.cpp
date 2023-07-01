#include "player.h"

Player::Player(const int &pos_x,const int &pos_y,const sf::Color &c, const int &up_limit,const int&down_limit)
{
    Striker *striker=new Striker(50,sf::Vector2f(pos_x,pos_y),400,400,up_limit,down_limit);
    striker_=striker;
    striker_->setFillColor(c);
    striker_->setOutlineThickness(5);
    striker_->setOutlineColor(sf::Color::Black);
    striker_->setOrigin(striker->getRadius(),striker->getRadius());

    Striker *striker2=new Striker(20,sf::Vector2f(pos_x,pos_y),400,400,up_limit,down_limit);
    striker_internal_=striker2;
    striker_internal_->setFillColor(c);
    striker_internal_->setOutlineThickness(5);
    striker_internal_->setOutlineColor(sf::Color::Black);
    striker_internal_->setOrigin(striker2->getRadius(),striker2->getRadius());
}

Player::Player()
{

}

Player::~Player()
{
    delete striker_;
    delete striker_internal_;
}

int Player::get_points() const
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

void Player::Player_animate(const sf::Time &elapsed, const bool &b, const sf::Vector2f &puck_position)
{
    striker_->animate(elapsed,b,puck_position);
    striker_internal_->setPosition(striker_->getPosition().x,striker_->getPosition().y);
}

void Player::reset(const sf::Vector2f &position)
{
    points_=0;
    striker_->setPosition(position);
}
