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

    // Collision with the table
    float next_position_x = this->getPosition().x + this->v_.x * elapsed.asSeconds();
    float next_position_y = this->getPosition().y + this->v_.y * elapsed.asSeconds();

    if(next_position_y < top_limit_ || next_position_y > bottom_limit_)
    {
        v_.y *= -1;
        play_sound = true;
    }

    if(next_position_x < this->left_limit_ || next_position_x > this->right_limit_)
    {
        v_.x *= -1;
        play_sound = true;
    }

    this->move(v_.x * elapsed.asSeconds(), v_.y * elapsed.asSeconds());

    // Collision with the strikers
    for(auto &striker :strikers)
    {
        float deltaX = this->getPosition().x - striker.getPosition().x;
        float deltaY = this->getPosition().y - striker.getPosition().y;
        float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
        float abs_dx = std::abs(striker.getPosition().x - striker.get_previous_position().x);
        float abs_dy = std::abs(striker.getPosition().y - striker.get_previous_position().y);

        constexpr int v0 = 80;
        constexpr int v1 = 100;

        if(distance <= this->getRadius() + striker.getRadius() + 5)
        {
            play_sound = true;
            this->v_.x = (this->getPosition().x < striker.getPosition().x) ? -std::abs(v0 + v1 * abs_dx) : std::abs(v0 + v1 * abs_dx);
            this->v_.y = (this->getPosition().y < striker.getPosition().y) ? -std::abs(v0 + v1 * abs_dy) : std::abs(v0 + v1 * abs_dy);
        }
    }

    return play_sound;
}


IsGoal Puck::check_goal()
{
    constexpr unsigned int X_MIN_GOAL = 180;
    constexpr unsigned int X_MAX_GOAL = 480;
    constexpr unsigned int Y_BLUE_GOAL = 70;
    constexpr unsigned int Y_RED_GOAL = 930;

    bool x_in_range = this->getPosition().x > X_MIN_GOAL && this->getPosition().x < X_MAX_GOAL;

    if(this->getPosition().y > Y_RED_GOAL && x_in_range == true)
    {
        this->reset(sf::Vector2f((X_MIN_GOAL+X_MAX_GOAL) / 2, (Y_BLUE_GOAL + Y_RED_GOAL) / 2 + 100));
        return BLUE_GOAL;
    }
    else if(this->getPosition().y < Y_BLUE_GOAL && x_in_range == true)
    {
        this->reset(sf::Vector2f((X_MIN_GOAL+X_MAX_GOAL) / 2, (Y_BLUE_GOAL + Y_RED_GOAL) / 2 - 100));
        return RED_GOAL;
    }

    return NO_GOAL;
}

void Puck::reset(const sf::Vector2f &position)
{
    this->v_ = sf::Vector2f(-150.f, 0.f);
    this->setPosition(position);
}
