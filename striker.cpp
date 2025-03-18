#include "striker.h"

Striker::Striker (const int &r, const sf::Vector2f &position, const int &up,const int &down)
    : sf::CircleShape(r), up_limit_(up), down_limit_(down)
{
    setPosition(position);
}
void Striker::animate(const sf::Time &elapsed, const bool& arrows, const sf::Vector2f &puck_position, const float &puck_radius)
{
    sf::Keyboard::Key keys[4];
    keys[0] = (arrows == true) ? sf::Keyboard::Up    : sf::Keyboard::W;
    keys[1] = (arrows == true) ? sf::Keyboard::Down  : sf::Keyboard::S;
    keys[2] = (arrows == true) ? sf::Keyboard::Left  : sf::Keyboard::A;
    keys[3] = (arrows == true) ? sf::Keyboard::Right : sf::Keyboard::D;

    float dx = this->getPosition().x - puck_position.x;
    float dy = this->getPosition().y - puck_position.y;
    float distance = sqrt(pow(dx, 2) + pow(dy, 2));

    if(distance > puck_radius + this->getRadius())
    {
        if(sf::Keyboard::isKeyPressed(keys[0]) && getPosition().y > up_limit_)
            this->move(0, -std::abs(v_.y * elapsed.asSeconds()));
        if(sf::Keyboard::isKeyPressed(keys[1]) && getPosition().y < down_limit_)
            this->move(0, std::abs(v_.y * elapsed.asSeconds()));
        if(sf::Keyboard::isKeyPressed(keys[2]) && this->getPosition().x > left_limit_ + this->getRadius())
            this->move(-std::abs(v_.x * elapsed.asSeconds()), 0);
        if(sf::Keyboard::isKeyPressed(keys[3]) && getPosition().x < right_limit_ -this->getRadius())
            this->move(std::abs(v_.x * elapsed.asSeconds()), 0);
    }
}

void Striker::set_previous_position(const sf::Vector2f &p)
{
    previous_position_ = p;
}

sf::Vector2f Striker::get_previous_position()
{
    return previous_position_;
}
