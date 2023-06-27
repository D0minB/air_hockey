#include "puck.h"
#include <vector>

Puck::Puck()
{

}

Puck::Puck(const float &r, const sf::Vector2f &position)
    : sf::CircleShape(r){
    this->setPosition(position);
    this->setFillColor(sf::Color(255,140,0));
    this->setOrigin(this->getRadius(),this->getRadius());
}

bool Puck::animate(const sf::Time &elapsed, std::vector<Striker> &strikers)
{
    bool play_sound = false;

    //COLLISION WITH STRIKER
    sf::FloatRect puck_bounds = this->getGlobalBounds();
    for(auto &s :strikers)
    {
        sf::FloatRect striker_bounds = s.getGlobalBounds();

        float dx = s.getPosition().x - s.get_previous_position().x;
        float dy = s.getPosition().y - s.get_previous_position().y;
        float abs_dx = std::abs(dx);
        float abs_dy = std::abs(dy);

        const int v0 = 80;
        const int v1 = 100;

        if(puck_bounds.intersects(striker_bounds)){
            play_sound = true;
            this->v_x_ = (this->getPosition().x < s.getPosition().x) ? -std::abs(v0 + v1 * abs_dx) : std::abs(v0 + v1 * abs_dx);
            this->v_y_ = (this->getPosition().y < s.getPosition().y) ? -std::abs(v0 + v1 * abs_dy) : std::abs(v0 + v1 * abs_dy);
        }
    }

    //COLLISION WITH TABLE
    if(this->getPosition().y<up_limit_ || this->getPosition().y>down_limit_)
    {
        v_y_ = -v_y_;
        play_sound=true;
    }
    if(this->getPosition().x < this->left_limit_ + this->getRadius() || this->getPosition().x > this->right_limit_ - this->getRadius())
    {
        v_x_ = -v_x_;
        play_sound=true;
    }

    this->move(v_x_ * elapsed.asSeconds(),v_y_ * elapsed.asSeconds());

    return play_sound;
}


int Puck::check_goal()
{
    const int MIN_GOAL = 180;
    const int MAX_GOAL = 390;

    if(this->getPosition().x > MIN_GOAL && this->getPosition().x < MAX_GOAL)
    {
        if(this->getPosition().y > this->down_limit_)
        {
            this->reset(sf::Vector2f(280,425+100));
            return 1;
        }
        else if(this->getPosition().y < this->up_limit_)
        {
            this->reset(sf::Vector2f(280,425-100));
            return 2;
        }
    }
    return 0;
}

void Puck::reset(const sf::Vector2f &position)
{
    this->v_x_ = -150;
    this->v_y_ = 0;
    this->setPosition(position);
}
