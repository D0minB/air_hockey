#include "puck.h"
#include <vector>

Puck::Puck()
{

}

Puck::Puck(const float &r, const sf::Vector2f &position)
    : sf::CircleShape(r)
{
    this->setPosition(position);
}

bool Puck::animate(const sf::Time &elapsed, std::vector<Striker> &strikers)
{
    bool play_sound=false;

    for(auto &s :strikers)
    {
        float distance = sqrt(pow(this->getPosition().x-s.getPosition().x,2)+pow(this->getPosition().y-s.getPosition().y,2));

        if(distance<=s.getRadius()+this->getRadius()+10) //10 - s outline is 5 and this outline is 5
        {
            play_sound=true;
            if(this->getPosition().x<s.getPosition().x)
            {
               // this->v_x_=-std::abs(this->v_x_);
                this->v_x_=-std::abs(100+50*std::abs(s.getPosition().x-s.get_previous_position().x));
            }
            if(this->getPosition().x>s.getPosition().x)
            {
                //this->v_x_=std::abs(this->v_x_);
                 this->v_x_=std::abs(100+50*std::abs(s.getPosition().x-s.get_previous_position().x));
            }
            if(this->getPosition().y<s.getPosition().y)
            {
               // this->v_y_=-std::abs(this->v_y_);
                this->v_y_=-std::abs(100+50*std::abs(s.getPosition().y-s.get_previous_position().y));

                //this->move(0,-(90-distance));
            }
            if(this->getPosition().y>s.getPosition().y)
            {
               // this->v_y_=std::abs(this->v_y_);
                this->v_y_=std::abs(100+50*std::abs(s.getPosition().y-s.get_previous_position().y));
                //this->move(0,90-distance);
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

void Puck::reset_velocity()
{
    v_x_=0;
    v_y_=0;
}

int Puck::check_goal()
{
    if(this->getPosition().x>180 && this->getPosition().x<390)
    {
        if(this->getPosition().y>785)
        {
            this->setPosition(sf::Vector2f(280,425+100));
            this->reset_velocity();
            return 1;
        }
        else if(this->getPosition().y<55)
        {
            this->setPosition(sf::Vector2f(280,425-100));
            this->reset_velocity();
            return 2;
        }
    }
    return 0;
}

void Puck::reset(const sf::Vector2f &position)
{
    v_x_=-150;
    v_y_=0;
    this->setPosition(position);
}
