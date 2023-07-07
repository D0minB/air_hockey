#include "hockeygame.h"

HockeyGame::HockeyGame(const int &W,const int &H) : window_(sf::VideoMode(W, H), "AIR HOCKEY")
{
    window_.setFramerateLimit(60);
    window_.setPosition(sf::Vector2i(0,0));

    //ICON
    if (!icon_.loadFromFile("resources/images/icon.png"))
        std::cerr << "Could not load icon" << std::endl;
    else
        window_.setIcon(icon_.getSize().x, icon_.getSize().y, icon_.getPixelsPtr());

    //TABLE
    if (!texture_table_->loadFromFile("resources/images/table.png"))
        std::cerr << "Could not load texture of table" << std::endl;
    else
    {
        texture_table_->setSmooth(true);
        sprite_table_->setTexture(*texture_table_);
    }

    //BUTTONS
    if (!texture_button_->loadFromFile("resources/images/button.png"))
        std::cerr << "Could not load texture of button" << std::endl;
    else
    {
        texture_button_->setSmooth(true);

        for(int i=0; i<2; i++)
        {
            std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(220,640+i*80),sf::Vector2f(1,0.7));
            menu_buttons_.emplace_back(std::move(button));
        }
        for(int i=0; i<2; i++)
        {
            std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(190,640+i*80),sf::Vector2f(1.2,0.7));
            end_buttons_.emplace_back(std::move(button));
        }

        // SETTINGS BUTTONS - FIRST COLUMN
        for(int i=0; i<2; i++)
        {
            std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(75,300+i*100),sf::Vector2f(1,0.7));
            settings_buttons_.emplace_back(std::move(button));
        }
        // SETTINGS BUTTONS - SECOND COLUMN
        for(int i=0; i<2; i++)
        {
            std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(375,300+i*100),sf::Vector2f(1,0.7));
            settings_buttons_.emplace_back(std::move(button));
        }
        // SETTINGS BUTTONS - BUTTON "OK"
        std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(220,650),sf::Vector2f(1,0.7));
        settings_buttons_.emplace_back(std::move(button));
    }

    //FONT
    if(!ttf->loadFromFile("resources/fonts/Roboto-Black.ttf"))
        std::cerr << "Font not load" << std::endl;

    //SOUNDS
    if (!cling_->openFromFile("resources/sounds/cling.wav"))
        std::cerr << "Cling not load" << std::endl;
    if (!goal_sound_->openFromFile("resources/sounds/goal.wav"))
         std::cerr << "Goal sound not load" << std::endl;
    if (!music_after_match_->openFromFile("resources/sounds/safe_and_sound.wav"))
        std::cerr << "Music after match not load" << std::endl;
    else
    {
        music_after_match_->setVolume(50.f);
        music_after_match_->setPlayingOffset(sf::seconds(15.f));
    }
}

sf::Text HockeyGame::print_text(const std::unique_ptr<sf::Font> &ttf, const std::string &txt, const int &size, const sf::Color &color, const sf::Vector2f &position)
{
    sf::Text text = sf::Text(txt,*ttf);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(position);
    return text;
}

std::unique_ptr<sf::Sprite> HockeyGame::add_button(const std::unique_ptr<sf::Texture> &texture_button, const sf::Vector2f &position, const sf::Vector2f &scale)
{
    std::unique_ptr<sf::Sprite> button_sprite = std::make_unique<sf::Sprite>();
    button_sprite->setTexture(*texture_button);
    button_sprite->setScale(scale);
    button_sprite->setPosition(position);
    return button_sprite;
}

void HockeyGame::draw()
{
    elapsed_ = clock.restart();

    if(elapsed_.asMilliseconds() > 20)
        std::cout << "Frame time: " << elapsed_.asMilliseconds() << " ms" << std::endl;

    window_.clear();
    window_.draw(*sprite_table_);

    switch(state_)
    {
    case GameState::menu:
        this->draw_menu();
        break;
    case GameState::intro:
        this->draw_intro();
        break;
    case GameState::match:
        this->draw_match();    
        break;
    case GameState::settings:
        this->draw_settings();
        break;
    case GameState::after_match:
        this->draw_after_match();
        break;
    }

    window_.display();
}

void HockeyGame::draw_after_match()
{
    int bluePoints = player_blue_->get_points();
    int redPoints = player_red_->get_points();

    std::stringstream ss;
    sf::Color c;

    if (bluePoints > redPoints)
    {
        ss << " BLUE PLAYER WINS\n\t\t\t   " << bluePoints << "-" << redPoints;
        c = sf::Color::Blue;
    }
    else if (bluePoints < redPoints)
    {
        ss << " RED PLAYER WINS\n\t\t\t   " << redPoints << "-" << bluePoints;
        c = sf::Color::Red;
    }
    else
    {
        ss << " DRAW " << bluePoints << "-" << redPoints;
        c = sf::Color::Black;
    }

    sf::Text summary = print_text(ttf, ss.str(), 60, c, sf::Vector2f(60,200));
    sf::Text buttons_text= print_text(ttf,"NEW MATCH\n\n       CLOSE",35, sf::Color::White, sf::Vector2f(220,650));

    for(const auto &el : end_buttons_)
    {
        window_.draw(*el);
    }
    window_.draw(summary);
    window_.draw(buttons_text);
}

void HockeyGame::draw_match()
{
    ms_ += elapsed_.asMilliseconds();

    if(ms_>=1000 && !(remained_min_==0 && remained_sec_==0))
    {
        ms_=0;
        if(remained_sec_ == 0)
        {
            remained_min_ -= 1;
            remained_sec_ = 59;
        }
        else
            remained_sec_-=1;
    }

    //PLAYERS MOVE
    player_blue_->get_striker()->set_previous_position(player_blue_->get_striker()->getPosition());
    player_red_->get_striker()->set_previous_position(player_red_->get_striker()->getPosition());

    player_blue_->animate(elapsed_, 0, puck_->getPosition(), puck_->getRadius());
    player_red_->animate(elapsed_, 1, puck_->getPosition(), puck_->getRadius());

    std::vector<Striker> strikers={*player_blue_->get_striker(),*player_red_->get_striker()};

    //DISPLAY RESULT
    std::string result = std::to_string(player_blue_->get_points())+"\n"+std::to_string(player_red_->get_points());
    sf::Text result_text = print_text(ttf,result,120,sf::Color::Black,sf::Vector2f(570,350));

    if(time_limit_ > 0)
    {
        std::string time_string = std::to_string(remained_min_) + ":" + (remained_sec_ < 10 ? "0" : "") + std::to_string(remained_sec_);
        sf::Text remained_time = print_text(ttf,time_string,40,sf::Color::Black,sf::Vector2f(40,450));
        window_.draw(remained_time);
    }

    //PUCK ANIMATE
    bool play_cling = puck_->animate(elapsed_,strikers);

    //REFLECTION SOUND
    if(play_cling)
    {
        cling_->play();
        cling_->setPlayingOffset(sf::seconds(0.f));
    }

    IsGoal goal_info = puck_->check_goal();
    if(goal_info != NO_GOAL)
    {
        goal_sound_->play();
        goal_sound_->setPlayingOffset(sf::seconds(0.f));

        player_blue_->get_striker()->setPosition(330,190);
        player_red_->get_striker()->setPosition(330,810);

        if(goal_info == BLUE_GOAL)
            player_blue_->add_point();
        else if(goal_info == RED_GOAL)
            player_red_->add_point();
    }

    if(((player_blue_->get_points()==points_limit_ || player_red_->get_points()==points_limit_) && points_limit_>0)
            || (time_limit_>0 && remained_min_==0 && remained_sec_==0))
    {
        state_ = GameState::after_match;
        music_after_match_->play();
    }

    window_.draw(result_text);
    window_.draw(*puck_);
    window_.draw(*player_blue_->get_striker());
    window_.draw(*player_blue_->get_striker_internal());
    window_.draw(*player_red_->get_striker());
    window_.draw(*player_red_->get_striker_internal());
}

void HockeyGame::draw_menu()
{
    sf::Text title = print_text(ttf,"\tAIR\nHOCKEY\n\n",75,sf::Color::Black,sf::Vector2f(190,160));
    sf::Text buttons_text= print_text(ttf,"\tSTART\n\nSETTINGS",35,sf::Color::White,sf::Vector2f(245,650));

    for(const auto &el : menu_buttons_)
    {
        window_.draw(*el);
    }
    window_.draw(title);
    window_.draw(buttons_text);
}

void HockeyGame::draw_intro()
{
    sf::Text limit;
    sf::Text text;
    sf::Text text_WASD;
    sf::Text text_arrows;

    if(points_limit_ > 0) //POINTS LIMIT
        limit = print_text(ttf, "Points limit: " + std::to_string(points_limit_), 25, sf::Color::Black, sf::Vector2f(450,520));
    else //TIME LIMIT
        limit = print_text(ttf, "Time limit: " + std::to_string(time_limit_) + " min", 25, sf::Color::Black, sf::Vector2f(40,450));

    //BUTTONS
    std::vector<std::unique_ptr<sf::Sprite>> intro_buttons;
    intro_buttons.emplace_back(add_button(texture_button_, sf::Vector2f(125, 250), sf::Vector2f(0.15, 0.3)));
    intro_buttons.emplace_back(add_button(texture_button_, sf::Vector2f(125, 550), sf::Vector2f(0.15, 0.3)));
    for(int i=0; i<3; i++)
    {
        intro_buttons.emplace_back(add_button(texture_button_, sf::Vector2f(90 + i * 35, 280), sf::Vector2f(0.15, 0.3)));
        intro_buttons.emplace_back(add_button(texture_button_, sf::Vector2f(90 + i * 35, 580), sf::Vector2f(0.15, 0.3)));
    }

    //TEXT
    text = print_text(ttf, "Press ENTER to continue...", 30, sf::Color::Black, sf::Vector2f(150, 900));
    text_WASD = print_text(ttf, "\t W\nA   S   D", 25, sf::Color::White, sf::Vector2f(100, 250));
    text_arrows = print_text(ttf, "\t  ^\n<    v    >", 25, sf::Color::White, sf::Vector2f(100, 550));

    for(const auto &el:intro_buttons)
    {
        window_.draw(*el);
    }

    window_.draw(*player_red_->get_striker());
    window_.draw(*player_red_->get_striker_internal());
    window_.draw(*player_blue_->get_striker());
    window_.draw(*player_blue_->get_striker_internal());
    window_.draw(limit);
    window_.draw(text);
    window_.draw(text_WASD);
    window_.draw(text_arrows);
}

void HockeyGame::draw_settings()
{
    sf::Text text = print_text(ttf,"Game rules", 60, sf::Color::Black, sf::Vector2f(180,150));
    sf::Text text_5 = print_text(ttf,"5 points",35,sf::Color::White,sf::Vector2f(120,310));
    sf::Text text_7 = print_text(ttf,"7 points",35,sf::Color::White,sf::Vector2f(430,310));
    sf::Text text_2min = print_text(ttf,"2 minutes",35,sf::Color::White,sf::Vector2f(110,410));
    sf::Text text_3min = print_text(ttf,"3 minutes",35,sf::Color::White,sf::Vector2f(420,410));
    sf::Text text_ok = print_text(ttf,"SAVE",35,sf::Color::White,sf::Vector2f(290,660));

    if(points_limit_ == 5)
        text_5.setFillColor(sf::Color::Yellow);
    else if(points_limit_ == 7)
        text_7.setFillColor(sf::Color::Yellow);
    else if(time_limit_ == 2)
        text_2min.setFillColor(sf::Color::Yellow);
    else if(time_limit_ == 3)
        text_3min.setFillColor(sf::Color::Yellow);

    for(const auto &el : settings_buttons_)
    {
        window_.draw(*el);
    }
    window_.draw(text);
    window_.draw(text_5);
    window_.draw(text_7);
    window_.draw(text_2min);
    window_.draw(text_3min);
    window_.draw(text_ok);
}

void HockeyGame::loop()
{
    while (window_.isOpen()) {

        sf::Event event;
        window_.pollEvent(event);

        if (event.type == sf::Event::Closed)
            window_.close();

        if (event.type == sf::Event::Resized)
            window_.setSize(sf::Vector2u(660, 1080-90));

        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window_);

        if(state_ == GameState::menu)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                //BUTTON "START"
                if(menu_buttons_[0]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                {
                    state_ = GameState::intro;
                    remained_min_ = time_limit_;
                    remained_sec_ = 0;
                }
                //BUTTON "SETTINGS"
                else if(menu_buttons_[1]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                    state_ = GameState::settings;
            }

        }
        else if(state_ == GameState::settings)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                if(settings_buttons_[0]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                {
                    points_limit_ = 5; //BUTTON "5 points"
                    time_limit_ = 0;
                }
                else if(settings_buttons_[1]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                {
                    time_limit_ = 2; //BUTTON "2 minutes"
                    points_limit_ = 0;
                }
                else if(settings_buttons_[2]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                {
                    points_limit_ = 7; //BUTTON "7 points"
                    time_limit_ = 0;
                }
                else if(settings_buttons_[3]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                {
                    time_limit_ = 3; //BUTTON "3 minutes"
                    points_limit_ = 0;
                }
                else if(settings_buttons_[4]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                    state_ = GameState::menu; //BUTTON "OK"
            }
        }
        else if (state_ == GameState::intro && event.key.code == sf::Keyboard::Enter)
            state_ = GameState::match;

        else if(state_ == GameState::after_match)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                //BUTTON "NEW GAME"
                if(end_buttons_[0]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                {
                    puck_->reset(sf::Vector2f(550, 495));
                    player_blue_->reset(sf::Vector2f(330, 190));
                    player_red_->reset(sf::Vector2f(330, 810));
                    state_ = GameState::match;

                    remained_min_ = time_limit_;
                    remained_sec_ = 0;

                    music_after_match_->stop();
                    music_after_match_->setPlayingOffset(sf::seconds(15.f));
                }

                //BUTTON "CLOSE"
                else if(end_buttons_[1]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                    window_.close();
            }
        }

        this->draw();
    }
}
