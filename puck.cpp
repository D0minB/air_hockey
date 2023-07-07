#include "puck.h"

Puck::Puck(const float &r, const sf::Vector2f &position)
    : sf::CircleShape(r){
    this->setPosition(position);
    this->setFillColor(this->color_);
    this->setOrigin(this->getRadius(),this->getRadius());
    this->setOutlineColor(sf::Color(255,140,0));
    this->setOutlineThickness(3);
}

bool Puck::animate(const sf::Time &elapsed, std::vector<Striker> &strikers)
{
    bool play_sound = false;

    //COLLISION WITH STRIKER
    for(auto &s :strikers)
    {
        float deltaX = this->getPosition().x - s.getPosition().x;
        float deltaY = this->getPosition().y - s.getPosition().y;
        float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
        float abs_dx = std::abs(s.getPosition().x - s.get_previous_position().x);
        float abs_dy = std::abs(s.getPosition().y - s.get_previous_position().y);

        const int v0 = 80;
        const int v1 = 100;

        if(distance <= this->getRadius() + s.getRadius() + 5)
        {
            play_sound = true;
            this->v_.x = (this->getPosition().x < s.getPosition().x) ? -std::abs(v0 + v1 * abs_dx) : std::abs(v0 + v1 * abs_dx);
            this->v_.y = (this->getPosition().y < s.getPosition().y) ? -std::abs(v0 + v1 * abs_dy) : std::abs(v0 + v1 * abs_dy);
        }
    }

    //COLLISION WITH TABLE
    if(this->getPosition().y<up_limit_ || this->getPosition().y>down_limit_)
    {
        v_.y = -v_.y;
        play_sound = true;
    }
    if(this->getPosition().x < this->left_limit_ + this->getRadius())
    {
        this->setPosition(sf::Vector2f(left_limit_ + 1.2 * this->getRadius(), this->getPosition().y));
        v_.x = -v_.x;
        play_sound = true;
    }
    else if(this->getPosition().x > this->right_limit_ - this->getRadius())
    {
        this->setPosition(sf::Vector2f(right_limit_ - 1.2 * this->getRadius(), this->getPosition().y));
        v_.x = -v_.x;
        play_sound = true;
    }

    this->move(v_.x * elapsed.asSeconds(), v_.y * elapsed.asSeconds());

    return play_sound;
}


IsGoal Puck::check_goal()
{
    const int MIN_GOAL = 180;
    const int MAX_GOAL = 480;

    if(this->getPosition().x > MIN_GOAL && this->getPosition().x < MAX_GOAL)
    {
        if(this->getPosition().y > this->down_limit_)
        {
            this->reset(sf::Vector2f((MIN_GOAL+MAX_GOAL) / 2, (this->up_limit_ + this->down_limit_) / 2 + 100));
            return BLUE_GOAL;
        }
        else if(this->getPosition().y < this->up_limit_)
        {
            this->reset(sf::Vector2f((MIN_GOAL+MAX_GOAL) / 2, (this->up_limit_ + this->down_limit_) / 2 - 100));
            return RED_GOAL;
        }
    }
    return NO_GOAL;
}

void Puck::reset(const sf::Vector2f &position)
{
    this->v_ = sf::Vector2f(-150.f, 0.f);
    this->setPosition(position);
}
