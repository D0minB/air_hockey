#include "player.h"

Player::Player(const int &pos_x,const int &pos_y,const sf::Color &c, const int &up_limit,const int&down_limit)
{
    Circle *striker=new Circle(50,sf::Vector2f(pos_x,pos_y),250,250,up_limit,down_limit);
    striker_=striker;
    striker_->setFillColor(c);
    striker_->setOutlineThickness(5);
    striker_->setOutlineColor(sf::Color::Black);
    striker_->setOrigin(50,50);

    Circle *striker2=new Circle(20,sf::Vector2f(pos_x,pos_y),250,250,up_limit,down_limit);
    striker_internal_=striker2;
    striker_internal_->setFillColor(c);
    striker_internal_->setOutlineThickness(5);
    striker_internal_->setOutlineColor(sf::Color::Black);
    striker_internal_->setOrigin(20,20);
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

Circle* Player::get_striker() const
{
    return striker_;
}

Circle* Player::get_striker_internal() const
{
    return striker_internal_;
}

void Player::Player_animate(const sf::Time &elapsed, const bool &b)
{
    striker_->animate(elapsed,b);
    striker_internal_->setPosition(striker_->getPosition().x,striker_->getPosition().y);
}







