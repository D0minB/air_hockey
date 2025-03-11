#pragma once
#include "player.h"
#include "puck.h"

enum class GameState
{
    menu,
    settings,
    intro,
    match,
    after_match
};

class HockeyGame
{
private:
    sf::Image icon_;
    std::unique_ptr<sf::Texture> texture_table_ = std::make_unique<sf::Texture>();
    std::unique_ptr<sf::Sprite> sprite_table_ = std::make_unique<sf::Sprite>();
    std::unique_ptr<sf::Texture> texture_button_ = std::make_unique<sf::Texture>();
    std::unique_ptr<sf::Sprite> sprite_button_ = std::make_unique<sf::Sprite>();
    std::unique_ptr<sf::Music> cling_ = std::make_unique<sf::Music>();
    std::unique_ptr<sf::Music> music_after_match_ = std::make_unique<sf::Music>();
    std::unique_ptr<sf::Music> goal_sound_ = std::make_unique<sf::Music>();
    std::unique_ptr<sf::Font> ttf = std::make_unique<sf::Font>();
    std::vector<std::unique_ptr<sf::Sprite>> menu_buttons_;
    std::vector<std::unique_ptr<sf::Sprite>> end_buttons_;
    std::vector<std::unique_ptr<sf::Sprite>> settings_buttons_;
    std::unique_ptr<Player> player_blue_ = std::make_unique<Player>(330, 190, sf::Color(0,0,255), sf::Color::Black, 75, 440);
    std::unique_ptr<Player> player_red_ = std::make_unique<Player>(330, 810, sf::Color::Red, sf::Color::Black, 550, 910);
    std::unique_ptr<Puck> puck_ = std::make_unique<Puck>(30,sf::Vector2f(550,495));

    sf::RenderWindow window_;
    sf::Clock clock;
    sf::Time elapsed_;

    GameState state_ = GameState::menu;

    //settings
    unsigned int points_limit_ = 7;
    unsigned int time_limit_ = 0; // in minutes
    unsigned int ms_ = 0;
    unsigned int remained_min_ = time_limit_;
    unsigned int remained_sec_ = 0;

public:
    HockeyGame(const int &W,const int &H);

    sf::Text print_text(const std::unique_ptr<sf::Font> &ttf,const std::string& s,const int &size, const sf::Color &color, const sf::Vector2f &position);
    std::unique_ptr<sf::Sprite> add_button(const std::unique_ptr<sf::Texture> &texture_button, const sf::Vector2f &position, const sf::Vector2f &scale);
    void draw();
    void draw_menu();
    void draw_settings();
    void draw_intro();
    void draw_after_match();
    void draw_match();
    void loop();
};
