#include "hockeygame.h"

HockeyGame::HockeyGame(const int &W,const int &H) : window_(sf::VideoMode(W, H), "AIR HOCKEY")
{
    window_.setFramerateLimit(60);

    //TABLE
    if (!texture_table_->loadFromFile("resources/images/table.png")) {
        std::cerr << "Could not load texture of table" << std::endl;
    }
    sprite_table_->setTexture(*texture_table_);
    sprite_table_->setScale(1.75, 1.75);

    //BUTTONS
    if (!texture_button_->loadFromFile("resources/images/button.png")) {
        std::cerr << "Could not load texture of button" << std::endl;
    }

    for(int i=0; i<2; i++)
    {
        std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(165,450+i*55),sf::Vector2f(1,0.5));
        menu_buttons_.emplace_back(std::move(button));
    }

    for(int i=0; i<2; i++)
    {
        std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(155,490+i*55),sf::Vector2f(1,0.5));
        end_buttons_.emplace_back(std::move(button));
    }

    // SETTINGS BUTTONS - FIRST COLUMN
    for(int i=0; i<2; i++)
    {
        std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(230,260+i*55),sf::Vector2f(0.6,0.5));
        settings_buttons_.emplace_back(std::move(button));
    }
    // SETTINGS BUTTONS - SECOND COLUMN
    for(int i=0; i<2; i++)
    {
        std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(380,260+i*55),sf::Vector2f(0.6,0.5));
        settings_buttons_.emplace_back(std::move(button));
    }
    // SETTINGS BUTTONS - BUTTON "OK"
    std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(250,650),sf::Vector2f(0.3,0.5));
    settings_buttons_.emplace_back(std::move(button));

    //FONT
    if(!ttf->loadFromFile("resources/fonts/Roboto-Black.ttf"))
    {
        std::cerr << "Font not load" << std::endl;
    }

    //SOUNDS
    if (!cling_->openFromFile("resources/sounds/cling.wav"))
    {
        std::cerr << "Cling not load" << std::endl;
    }
    if (!goal_sound_->openFromFile("resources/sounds/goal.wav"))
    {
        std::cerr << "Goal sound not load" << std::endl;
    }
    if (!music_after_match_->openFromFile("resources/sounds/safe_and_sound.wav"))
    {
        std::cerr << "Music after match not load" << std::endl;
    }
    music_after_match_->setPlayingOffset(sf::seconds(15.f));
}

std::unique_ptr<sf::Text> HockeyGame::print_text(const std::unique_ptr<sf::Font> &ttf, const std::string &s, const int &size, const sf::Color &color, const sf::Vector2f &position)
{
    std::unique_ptr<sf::Text> text = std::make_unique<sf::Text>(s,*ttf);
    text->setCharacterSize(size);
    text->setFillColor(color);
    text->setPosition(position);
    return text;
}

std::unique_ptr<sf::Sprite> HockeyGame::add_button(const std::unique_ptr<sf::Texture> &texture_button, const sf::Vector2f &position, const sf::Vector2f &scale)
{
    std::unique_ptr<sf::Sprite> button = std::make_unique<sf::Sprite>();
    button->setTexture(*texture_button);
    button->setScale(scale);
    button->setPosition(position);
    return button;
}

void HockeyGame::draw()
{
    elapsed_ = clock.restart();
    window_.clear(sf::Color::Black);
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
    std::string text = "CONGRATULATIONS!\n\n";

    if(player_blue_->get_points()==points_limit_ && points_limit_>0)
    {
        text+=" BLUE PLAYER WINS\n\t\t\t   "+std::to_string(player_blue_->get_points())+"-"+std::to_string(player_red_->get_points());
    }
    else if(player_red_->get_points()==points_limit_ && points_limit_>0)
    {
        text+=" RED PLAYER WINS\n\t\t\t   "+std::to_string(player_red_->get_points())+"-"+std::to_string(player_blue_->get_points());
    }

    if(time_limit_>0)
    {
        if(player_blue_->get_points() > player_red_->get_points())
        {
            text+=" BLUE PLAYER WINS\n\t\t\t   "+std::to_string(player_blue_->get_points())+"-"+std::to_string(player_red_->get_points());
        }
        else if(player_blue_->get_points() < player_red_->get_points())
        {
            text+=" RED PLAYER WINS\n\t\t\t   "+std::to_string(player_red_->get_points())+"-"+std::to_string(player_blue_->get_points());
        }
        else
        {
            text+=" DRAW\t\t\t   "+std::to_string(player_red_->get_points())+"-"+std::to_string(player_blue_->get_points());
        }
    }

    std::string buttons_string = "NEW MATCH\n\n     CLOSE";

    std::unique_ptr<sf::Text> summary = print_text(ttf,text,45,sf::Color::Black,sf::Vector2f(80,160));
    std::unique_ptr<sf::Text> buttons_text = print_text(ttf,buttons_string,25,sf::Color::White,sf::Vector2f(205,495));

    for(const auto &el : end_buttons_)
    {
        window_.draw(*el);
    }
    window_.draw(*summary);
    window_.draw(*buttons_text);
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
        {
            remained_sec_-=1;
        }
    }

    //PLAYERS MOVE
    player_blue_->get_striker()->set_previous_position(player_blue_->get_striker()->getPosition());
    player_red_->get_striker()->set_previous_position(player_red_->get_striker()->getPosition());

    player_blue_->Player_animate(elapsed_,0,puck_->getPosition());
    player_red_->Player_animate(elapsed_,1,puck_->getPosition());

    std::vector<Striker> strikers={*player_blue_->get_striker(),*player_red_->get_striker()};

    //DISPLAY RESULT
    std::string result = std::to_string(player_blue_->get_points())+"\n"+std::to_string(player_red_->get_points());
    std::unique_ptr<sf::Text> result_text = print_text(ttf,result,120,sf::Color::Black,sf::Vector2f(470,280));

    if(time_limit_ > 0)
    {
        std::string time_string = std::to_string(remained_min_) + ":" + (remained_sec_ < 10 ? "0" : "") + std::to_string(remained_sec_);
        std::unique_ptr<sf::Text> remained_time = print_text(ttf,time_string,40,sf::Color::Black,sf::Vector2f(25,375));
        window_.draw(*remained_time);
    }

    //PUCK ANIMATE
    bool play_cling = puck_->animate(elapsed_,strikers);

    //REFLECTION SOUND
    if(play_cling)
    {
        cling_->play();
        cling_->setPlayingOffset(sf::seconds(0.f));
    }
    //GOAL 0-no goal, 1-player1 scores, 2-player2 scores
    int goal = puck_->check_goal();
    if(goal==1 || goal==2)
    {
        goal_sound_->play();
        goal_sound_->setPlayingOffset(sf::seconds(0.f));

        player_blue_->get_striker()->setPosition(280,120);
        player_red_->get_striker()->setPosition(280,710);

        if(goal==1)
        {
            player_blue_->add_point();
        }
        if(goal==2)
        {
            player_red_->add_point();
        }
    }

    if(((player_blue_->get_points()==points_limit_ || player_red_->get_points()==points_limit_) && points_limit_>0)
            || (time_limit_>0 && remained_min_==0 && remained_sec_==0))
    {
        state_=GameState::after_match;
        music_after_match_->play();
    }
    window_.draw(*result_text);
    window_.draw(*puck_);
    window_.draw(*player_blue_->get_striker());
    window_.draw(*player_blue_->get_striker_internal());
    window_.draw(*player_red_->get_striker());
    window_.draw(*player_red_->get_striker_internal());
}

void HockeyGame::draw_menu()
{
    std::unique_ptr<sf::Text> title = print_text(ttf,"\tAIR\nHOCKEY\n\n",75,sf::Color::Black,sf::Vector2f(140,160));
    std::unique_ptr<sf::Text> buttons_text= print_text(ttf,"\tSTART\n\nSETTINGS",25,sf::Color::White,sf::Vector2f(220,455));

    for(const auto &el : menu_buttons_)
    {
        window_.draw(*el);
    }
    window_.draw(*title);
    window_.draw(*buttons_text);
}

void HockeyGame::draw_intro()
{
    std::unique_ptr<sf::Text> limit;
    if(points_limit_>0)
    {
        limit = print_text(ttf,"Points limit: "+std::to_string(points_limit_),25,sf::Color::Black,sf::Vector2f(375,430));
    }

    else if(time_limit_>0)
    {
        limit = print_text(ttf,"Time limit: "+std::to_string(time_limit_)+" min",25,sf::Color::Black,sf::Vector2f(25,380));
    }
    std::vector<std::unique_ptr<sf::Sprite>> intro_buttons;

    std::unique_ptr<sf::Sprite> button = add_button(texture_button_,sf::Vector2f(125,200),sf::Vector2f(0.15,0.3));
    intro_buttons.emplace_back(std::move(button));

    for(int i=0; i<3; i++)
    {
        button = add_button(texture_button_,sf::Vector2f(90+i*35,230),sf::Vector2f(0.15,0.3));
        intro_buttons.emplace_back(std::move(button));
    }

    button = add_button(texture_button_,sf::Vector2f(125,500),sf::Vector2f(0.15,0.3));
    intro_buttons.emplace_back(std::move(button));

    for(int i=0; i<3; i++)
    {
        button = add_button(texture_button_,sf::Vector2f(90+i*35,530),sf::Vector2f(0.15,0.3));
        intro_buttons.emplace_back(std::move(button));
    }

    std::unique_ptr<sf::Text> text = print_text(ttf,"Press ENTER to continue...",30,sf::Color::Black,sf::Vector2f(100,780));
    std::unique_ptr<sf::Text> text_WASD = print_text(ttf,"\t W\nA   S   D",25,sf::Color::White,sf::Vector2f(100,200));
    std::unique_ptr<sf::Text> text_arrows = print_text(ttf,"\t  ^\n<    v    >",25,sf::Color::White,sf::Vector2f(100,200+300));

    for(const auto &el:intro_buttons)
    {
        window_.draw(*el);
    }

    window_.draw(*limit);
    window_.draw(*text);
    window_.draw(*text_WASD);
    window_.draw(*text_arrows);
    window_.draw(*player_red_->get_striker());
    window_.draw(*player_red_->get_striker_internal());
    window_.draw(*player_blue_->get_striker());
    window_.draw(*player_blue_->get_striker_internal());
}

void HockeyGame::draw_settings()
{
    std::unique_ptr<sf::Text> text = print_text(ttf,"Game\nlimit:",30,sf::Color::Black,sf::Vector2f(50,265));
    std::unique_ptr<sf::Text> text_5 = print_text(ttf,"5 points",25,sf::Color::White,sf::Vector2f(250,270));
    std::unique_ptr<sf::Text> text_7=print_text(ttf,"7 points",25,sf::Color::White,sf::Vector2f(405,270));
    std::unique_ptr<sf::Text> text_2min=print_text(ttf,"2 minutes",25,sf::Color::White,sf::Vector2f(240,320));
    std::unique_ptr<sf::Text> text_3min=print_text(ttf,"3 minutes",25,sf::Color::White,sf::Vector2f(395,320));
    std::unique_ptr<sf::Text> text_ok=print_text(ttf,"OK",25,sf::Color::White,sf::Vector2f(270,655));

    if(points_limit_==5)
    {
        text_5->setFillColor(sf::Color::Yellow);
    }
    else if(points_limit_==7)
    {
        text_7->setFillColor(sf::Color::Yellow);
    }
    else if(time_limit_==2)
    {
        text_2min->setFillColor(sf::Color::Yellow);
    }
    else if(time_limit_==3)
    {
        text_3min->setFillColor(sf::Color::Yellow);
    }

    std::vector<std::unique_ptr<sf::Text>> buttons_text;
    buttons_text.emplace_back(std::move(text));
    buttons_text.emplace_back(std::move(text_5));
    buttons_text.emplace_back(std::move(text_7));
    buttons_text.emplace_back(std::move(text_2min));
    buttons_text.emplace_back(std::move(text_3min));
    buttons_text.emplace_back(std::move(text_ok));

    for(const auto &el : settings_buttons_)
    {
        window_.draw(*el);
    }

    for(const auto &el : buttons_text)
    {
        window_.draw(*el);
    }
}

void HockeyGame::loop()
{
    while (window_.isOpen()) {

        sf::Event event;
        window_.pollEvent(event);

        if (event.type == sf::Event::Closed)
        {
            window_.close();
        }

        if (event.type == sf::Event::Resized)
        {
            window_.setSize(sf::Vector2u(560,840));
        }

        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window_);
        if(state_==GameState::menu)
        {
            //BUTTON "START"
            if(menu_buttons_[0]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y) && event.mouseButton.button == sf::Mouse::Left)
            {
                state_=GameState::intro;
                remained_min_=time_limit_;
                remained_sec_=0;
            }

            //BUTTON "SETTINGS"
            if(menu_buttons_[1]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y) && event.mouseButton.button == sf::Mouse::Left)
            {
                state_=GameState::settings;
            }
        }
        if(state_==GameState::settings)
        {
            //BUTTON "5 points"
            if(settings_buttons_[0]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y) && event.mouseButton.button == sf::Mouse::Left)
            {
                points_limit_=5;
                time_limit_=0;
            }
            //BUTTON "2 minutes"
            else if(settings_buttons_[1]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y) && event.mouseButton.button == sf::Mouse::Left)
            {
                time_limit_=2;
                points_limit_=0;
            }
            //BUTTON "7 points"
            else if(settings_buttons_[2]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y) && event.mouseButton.button == sf::Mouse::Left)
            {
                points_limit_=7;
                time_limit_=0;
            }
            //BUTTON "3 minutes"
            else if(settings_buttons_[3]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y) && event.mouseButton.button == sf::Mouse::Left)
            {
                time_limit_=3;
                points_limit_=0;
            }
            //BUTTON "OK"
            else if(settings_buttons_[4]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y) && event.mouseButton.button == sf::Mouse::Left)
            {
                state_=GameState::menu;
            }
        }
        if (state_==GameState::intro && event.key.code == sf::Keyboard::Enter)
        {
            state_=GameState::match;
        }
        if(state_==GameState::after_match)
        {
            //BUTTON "NEW GAME"
            if(end_buttons_[0]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y) && event.mouseButton.button == sf::Mouse::Left)
            {
                puck_->reset(sf::Vector2f(280+220,425));
                player_blue_->reset(sf::Vector2f(280,120));
                player_red_->reset(sf::Vector2f(280,710));
                state_=GameState::match;

                remained_min_=time_limit_;
                remained_sec_=0;

                music_after_match_->stop();
                music_after_match_->setPlayingOffset(sf::seconds(15.f));
            }

            //BUTTON "CLOSE"
            if(end_buttons_[1]->getGlobalBounds().contains(mouse_pos.x,mouse_pos.y) && event.mouseButton.button == sf::Mouse::Left)
            {
                window_.close();
            }

        }

        this->draw();
    }
}
