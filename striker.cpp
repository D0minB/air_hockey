#include "striker.h"

Striker::Striker()
{

}

Striker::Striker (const int &r, const sf::Vector2f &position,const int& v_x,const int& v_y, const int&up,const int&down)
    : sf::CircleShape(r), v_x_(v_x),v_y_(v_y),up_limit_(up),down_limit_(down)
    {
        setPosition(position);
    }
void Striker::animate(const sf::Time &elapsed, const bool& arrows)
{
    if(arrows)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && getPosition().y>up_limit_)
        {
            move(0,-std::abs(v_y_*elapsed.asSeconds()));
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && getPosition().y<down_limit_)
        {
            move(0,std::abs(v_y_*elapsed.asSeconds()));
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && getPosition().x>25+this->getRadius())
        {
            move(-std::abs(v_x_*elapsed.asSeconds()),0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && getPosition().x<535-this->getRadius())
        {
            move(std::abs(v_x_*elapsed.asSeconds()),0);
        }
    }
    else
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && getPosition().y>up_limit_)
        {
            move(0,-std::abs(v_y_*elapsed.asSeconds()));
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && getPosition().y<down_limit_)
        {
            move(0,std::abs(v_y_*elapsed.asSeconds()));
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && getPosition().x>25+this->getRadius())
        {
            move(-std::abs(v_x_*elapsed.asSeconds()),0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && getPosition().x<535-this->getRadius())
        {
            move(std::abs(v_x_*elapsed.asSeconds()),0);
        }
    }
}
