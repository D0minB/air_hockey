#include "puck.h"
#include <vector>

Puck::Puck()
{

}

Puck::Puck(const float &r, const sf::Vector2f &position, const int &v_x, const int &v_y)
    : sf::CircleShape(r),v_x_(v_x),v_y_(v_y)
{
    this->setPosition(position);
}

bool Puck::animate(const sf::Time &elapsed, const std::vector<Striker> &strikers)
{
    bool play_sound=false;

    for(auto const &s :strikers)
    {
        float distance = sqrt(pow(this->getPosition().x-s.getPosition().x,2)+pow(this->getPosition().y-s.getPosition().y,2));

        if(distance<=s.getRadius()+this->getRadius()+10) //10 - s outline is 5 and this outline is 5
        {
            play_sound=true;
            if(this->getPosition().x<s.getPosition().x)
            {
                this->v_x_=-std::abs(this->v_x_);
            }
            if(this->getPosition().x>s.getPosition().x)
            {
                this->v_x_=std::abs(this->v_x_);
            }
            if(this->getPosition().y<s.getPosition().y)
            {
                this->v_y_=-std::abs(this->v_y_);
                this->move(0,-(90-distance));
            }
            if(this->getPosition().y>s.getPosition().y)
            {
                this->v_y_=std::abs(this->v_y_);
                this->move(0,90-distance);
            }
        }

    }

    if(this->getPosition().y<up_limit_)
    {
        v_y_=std::abs(v_y_);
        play_sound=true;
    }
    if(this->getPosition().y>down_limit_)
    {
        v_y_=-std::abs(v_y_);
        play_sound=true;
    }
    if(this->getPosition().x<30+this->getRadius())
    {
        v_x_=std::abs(v_x_);
        play_sound=true;
    }
    if(this->getPosition().x>535-this->getRadius())
    {
        v_x_=-std::abs(v_x_);
        play_sound=true;
    }

    this->move(v_x_*elapsed.asSeconds(),v_y_*elapsed.asSeconds());

    return play_sound;
}
