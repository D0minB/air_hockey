#include "striker.h"


Striker::Striker (const int &r, const sf::Vector2f &position, const int &up,const int &down)
    : sf::CircleShape(r), up_limit_(up), down_limit_(down)
{
    setPosition(position);
}


void Striker::animate(const sf::Time &elapsed, const bool &arrows, const sf::Vector2f &puck_position, const float &puck_radius)
{
    sf::Keyboard::Key keys[4];
    keys[0] = (arrows == true) ? sf::Keyboard::Up    : sf::Keyboard::W;
    keys[1] = (arrows == true) ? sf::Keyboard::Down  : sf::Keyboard::S;
    keys[2] = (arrows == true) ? sf::Keyboard::Left  : sf::Keyboard::A;
    keys[3] = (arrows == true) ? sf::Keyboard::Right : sf::Keyboard::D;

    // Calculate distance from puck to striker
    float x_movement = v_.x * elapsed.asSeconds();
    float y_movement = v_.y * elapsed.asSeconds();

    auto isCollisionFree = [&](float nextX, float nextY) -> bool
    {
        const float dx = nextX - puck_position.x;
        const float dy = nextY - puck_position.y;
        const float distance = std::sqrt(dx * dx + dy * dy);
        return (distance > puck_radius + this->getRadius());
    };

    if (sf::Keyboard::isKeyPressed(keys[0]) && this->getPosition().y > up_limit_)
    {
        float nextY = this->getPosition().y - y_movement;
        if (isCollisionFree(this->getPosition().x, nextY))
            this->move(0, -y_movement);
    }
    if (sf::Keyboard::isKeyPressed(keys[1]) && this->getPosition().y < down_limit_)
    {
        float nextY = this->getPosition().y + y_movement;
        if (isCollisionFree(this->getPosition().x, nextY))
            this->move(0, y_movement);
    }
    if (sf::Keyboard::isKeyPressed(keys[2]) && this->getPosition().x > left_limit_ + this->getRadius())
    {
        float nextX = this->getPosition().x - x_movement;
        if (isCollisionFree(nextX, this->getPosition().y))
            this->move(-x_movement, 0);
    }
    if (sf::Keyboard::isKeyPressed(keys[3]) && this->getPosition().x < right_limit_ - this->getRadius())
    {
        float nextX = this->getPosition().x + x_movement;
        if (isCollisionFree(nextX, this->getPosition().y))
            this->move(x_movement, 0);
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
