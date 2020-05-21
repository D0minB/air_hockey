#pragma once
#include <headers.h>
#include <striker.h>

class Puck :public sf::CircleShape
{
private:
    int v_x_=0;
    int v_y_=0;
    int up_limit_=55;
    int down_limit_=785;
public:
    Puck();
    Puck(const float &r, const sf::Vector2f &position,const int &v_x,const int&v_y);
    bool animate(const sf::Time &elapsed, const std::vector<Striker> &strikers);
//    void animate(const sf::Time &elapsed, const bool& arrows)
//    {
//        if(arrows)
//        {
//            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && getPosition().y>up_limit_)
//            {
//                move(0,-std::abs(v_y_*elapsed.asSeconds()));
//            }
//            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && getPosition().y<down_limit_)
//            {
//                move(0,std::abs(v_y_*elapsed.asSeconds()));
//            }
//            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && getPosition().x>25+this->getRadius())
//            {
//                move(-std::abs(v_x_*elapsed.asSeconds()),0);
//            }
//            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && getPosition().x<535-this->getRadius())
//            {
//                move(std::abs(v_x_*elapsed.asSeconds()),0);
//            }
//        }
//        else
//        {
//            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && getPosition().y>up_limit_)
//            {
//                move(0,-std::abs(v_y_*elapsed.asSeconds()));
//            }
//            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && getPosition().y<down_limit_)
//            {
//                move(0,std::abs(v_y_*elapsed.asSeconds()));
//            }
//            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && getPosition().x>25+this->getRadius())
//            {
//                move(-std::abs(v_x_*elapsed.asSeconds()),0);
//            }
//            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && getPosition().x<535-this->getRadius())
//            {
//                move(std::abs(v_x_*elapsed.asSeconds()),0);
//            }
//        }
//    }
};
