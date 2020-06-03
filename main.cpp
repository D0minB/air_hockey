#include "player.h"
#include "striker.h"
#include "puck.h"

sf::Text print(const sf::Font &ttf,const std::string& s,const int &size, const sf::Color &color, const sf::Vector2f &position)
{
    sf::Text text(s,ttf);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(position);

    return text;
}

sf::Sprite add_button(const sf::Texture &texture_button, const sf::Vector2f &position, const sf::Vector2f &scale)
{
        sf::Sprite button;
        button.setTexture(texture_button);
        button.setScale(scale);
        button.setPosition(position);
        return button;
}


int main() {
    sf::RenderWindow window(sf::VideoMode(560, 840), "AIR HOCKEY");
    window.setVerticalSyncEnabled(true); //frame limit

    //TABLE
    sf::Texture texture_table;
    if (!texture_table.loadFromFile("resources/images/table.png")) {
        std::cerr << "Could not load texture of table" << std::endl;
        return 1;
    }
    sf::Sprite sprite_table;
    sprite_table.setTexture(texture_table);
    sprite_table.setScale(1.75, 1.75);

    //BUTTONS
    sf::Texture texture_button;
    if (!texture_button.loadFromFile("resources/images/button.png")) {
        std::cerr << "Could not load texture of button" << std::endl;
        return 1;
    }

    std::vector<sf::Sprite> end_buttons;
    for(int i=0; i<2; i++)
    {
        sf::Sprite button = add_button(texture_button,sf::Vector2f(155,490+i*55),sf::Vector2f(1,0.5));
        end_buttons.emplace_back(button);
    }
    std::vector<sf::Sprite> menu_buttons;
    for(int i=0; i<2; i++)
    {
        sf::Sprite button = add_button(texture_button,sf::Vector2f(165,450+i*55),sf::Vector2f(1,0.5));
        menu_buttons.emplace_back(button);
    }
    std::vector<sf::Sprite> settings_buttons;
    // SETTINGS BUTTONS - FIRST COLUMN
    for(int i=0; i<2; i++)
    {
        sf::Sprite button = add_button(texture_button,sf::Vector2f(230,260+i*55),sf::Vector2f(0.6,0.5));
        settings_buttons.emplace_back(button);
    }
    // SETTINGS BUTTONS - SECOND COLUMN
    for(int i=0; i<2; i++)
    {
        sf::Sprite button = add_button(texture_button,sf::Vector2f(380,260+i*55),sf::Vector2f(0.6,0.5));
        settings_buttons.emplace_back(button);
    }
    // SETTINGS BUTTONS - BUTTON "OK"
    for(int i=0; i<1; i++)
    {
        sf::Sprite button = add_button(texture_button,sf::Vector2f(250,650),sf::Vector2f(0.3,0.5));
        settings_buttons.emplace_back(button);
    }

    //SOUNDS
    sf::Music cling;
    if (!cling.openFromFile("resources/sounds/cling.wav"))
    {
        std::cerr << "Cling not load" << std::endl;
        return 1;
    }

    sf::Music goal_sound;
    if (!goal_sound.openFromFile("resources/sounds/goal.wav"))
    {
        std::cerr << "Goal sound not load" << std::endl;
        return 1;
    }

    sf::Music music_after_match;
    if (!music_after_match.openFromFile("resources/sounds/safe_and_sound.wav"))
    {
        std::cerr << "Music after match not load" << std::endl;
        return 1;
    }
    music_after_match.setPlayingOffset(sf::seconds(15.f));

    //FONT
    sf::Font ttf;
    if(!ttf.loadFromFile("resources/fonts/Roboto-Black.ttf"))
    {
        std::cerr << "Font not load" << std::endl;
        return 1;
    }

    //PLAYERS - constructor
    Player player_blue(280,120,sf::Color::Blue,75,365);
    Player player_red(280,710,sf::Color::Red,480,770);

    //PUCK - constructor
    Puck puck(30,sf::Vector2f(500,425));

    sf::Clock clock;

    bool menu=1;
    bool settings=0;
    bool intro=0;
    bool match=0;
    bool end_of_match = 0;

    //settings
    int points_limit=7;
    int time_limit=0; // in minutes
    int ms=0;
    int remained_min=time_limit;
    int remained_sec=0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

             sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
            if(menu)
            {
                //BUTTON "START"
                if(menu_buttons[0].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    menu=false;
                    intro=true;
                }

                //BUTTON "SETTINGS"
                if(menu_buttons[1].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    menu=false;
                    settings=true;
                }
            }

            if(settings)
            {
                //BUTTON "5 points"
                if(settings_buttons[0].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    points_limit=5;
                    time_limit=0;
                }
                //BUTTON "2 minutes"
                else if(settings_buttons[1].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    time_limit=2;
                    points_limit=0;
                }
                //BUTTON "7 points"
                else if(settings_buttons[2].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    points_limit=7;
                    time_limit=0;
                }
                //BUTTON "3 minutes"
                else if(settings_buttons[3].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    time_limit=3;
                    points_limit=0;
                }
                //BUTTON "OK"
                else if(settings_buttons[4].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    settings=false;
                    menu=true;
                }
            }

            if (intro && event.key.code == sf::Keyboard::Enter)
            {
                    intro=false;
                    match=true;
            }

            if(end_of_match)
            {
                //BUTTON "NEW GAME"
                if(end_buttons[0].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    puck.reset(sf::Vector2f(280+220,425));
                    player_blue.reset(sf::Vector2f(280,120));
                    player_red.reset(sf::Vector2f(280,710));
                    end_of_match=false;
                    match=true;

                    remained_min=time_limit;
                    remained_sec=0;

                    music_after_match.stop();
                    music_after_match.setPlayingOffset(sf::seconds(15.f));
                }

                //BUTTON "CLOSE"
                if(end_buttons[1].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    window.close();
                }
            }
        }

        sf::Time elapsed = clock.restart();

        window.clear(sf::Color::White);
        window.draw(sprite_table);

        //MENU
        if(menu)
        {
            sf::Text title = print(ttf,"\tAIR\nHOCKEY\n\n",75,sf::Color::Black,sf::Vector2f(140,160));
            sf::Text buttons_text = print(ttf,"\tSTART\n\nSETTINGS",25,sf::Color::White,sf::Vector2f(220,455));

            for(const auto &el : menu_buttons)
            {
                window.draw(el);
            }
            window.draw(title);
            window.draw(buttons_text);
        }

        //SETTINGS
        if(settings)
        {
            sf::Text text = print(ttf,"Game\nlimit:",30,sf::Color::Black,sf::Vector2f(50,265));
            sf::Text text_5 = print(ttf,"5 points",25,sf::Color::White,sf::Vector2f(250,270));
            sf::Text text_7=print(ttf,"7 points",25,sf::Color::White,sf::Vector2f(405,270));
            sf::Text text_2min=print(ttf,"2 minutes",25,sf::Color::White,sf::Vector2f(240,320));
            sf::Text text_3min=print(ttf,"3 minutes",25,sf::Color::White,sf::Vector2f(395,320));
            sf::Text text_ok=print(ttf,"OK",25,sf::Color::White,sf::Vector2f(270,655));

            if(points_limit==5)
            {
                text_5.setFillColor(sf::Color::Yellow);
            }
            else if(points_limit==7)
            {
                text_7.setFillColor(sf::Color::Yellow);
            }
            else if(time_limit==2)
            {
                text_2min.setFillColor(sf::Color::Yellow);
            }
            else if(time_limit==3)
            {
                text_3min.setFillColor(sf::Color::Yellow);
            }

            std::vector<sf::Text> buttons_text={text,text_5,text_7,text_2min,text_3min,text_ok};

            for(const auto &el : settings_buttons)
            {
                window.draw(el);
            }

            for(const auto &el : buttons_text)
            {
                window.draw(el);
            }
        }
        //INTRO
        if(intro)
        {
            sf::Text limit;
            if(points_limit>0)
            {
                limit = print(ttf,"Points limit: "+std::to_string(points_limit),25,sf::Color::Black,sf::Vector2f(375,430));
            }

            else if(time_limit>0)
            {
                limit = print(ttf,"Time limit: "+std::to_string(time_limit)+" min",25,sf::Color::Black,sf::Vector2f(25,380));
            }

            sf::Text text = print(ttf,"Press ENTER to continue...",30,sf::Color::Black,sf::Vector2f(100,780));
            sf::Text text_WASD = print(ttf,"\t W\nA   S   D",25,sf::Color::White,sf::Vector2f(100,200));
            sf::Text text_arrows = print(ttf,"\t  ^\n<    v    >",25,sf::Color::White,sf::Vector2f(100,200+300));

            std::vector<sf::Text> texts={limit,text,text_WASD,text_arrows};

            std::vector<sf::Sprite> intro_buttons;

            sf::Sprite button = add_button(texture_button,sf::Vector2f(125,200),sf::Vector2f(0.15,0.3));
            intro_buttons.emplace_back(button);

            for(int i=0; i<3; i++)
            {
                button = add_button(texture_button,sf::Vector2f(90+i*35,230),sf::Vector2f(0.15,0.3));
                intro_buttons.emplace_back(button);
            }

            button = add_button(texture_button,sf::Vector2f(125,500),sf::Vector2f(0.15,0.3));
            intro_buttons.emplace_back(button);

            for(int i=0; i<3; i++)
            {
                button = add_button(texture_button,sf::Vector2f(90+i*35,530),sf::Vector2f(0.15,0.3));
                intro_buttons.emplace_back(button);
            }
            for(const auto &el:intro_buttons)
            {
                window.draw(el);
            }
            for(const auto &el:texts)
            {
                window.draw(el);
            }
            window.draw(*player_blue.get_striker());
            window.draw(*player_blue.get_striker_internal());
            window.draw(*player_red.get_striker());
            window.draw(*player_red.get_striker_internal());
        }

        //MATCH
        if(match)
        {
            ms+=elapsed.asMilliseconds();

            if(ms>=1000 && !(remained_min==0 && remained_sec==0))
            {
                ms=0;

                if(remained_sec==0)
                {
                    remained_min-=1;
                    remained_sec=59;
                }
                else
                {
                    remained_sec-=1;
                }

            }

            //PLAYERS MOVE
            player_blue.get_striker()->set_previous_position(player_blue.get_striker()->getPosition());
            player_red.get_striker()->set_previous_position(player_red.get_striker()->getPosition());

            player_blue.Player_animate(elapsed,0,puck.getPosition());
            player_red.Player_animate(elapsed,1,puck.getPosition());

            std::vector<Striker> strikers={*player_blue.get_striker(),*player_red.get_striker()};

            //DISPLAY RESULT
            std::string result = std::to_string(player_blue.get_points())+"\n"+std::to_string(player_red.get_points());
            sf::Text result_text = print(ttf,result,120,sf::Color::Black,sf::Vector2f(470,280));

            if(time_limit>0)
            {
                std::string time_string = std::to_string(remained_min)+":";

                if(remained_sec<10)
                {
                    time_string+="0";
                }
                time_string+=std::to_string(remained_sec);
                sf::Text remained_time = print(ttf,time_string,40,sf::Color::Black,sf::Vector2f(25,375));
                window.draw(remained_time);
            }

            //PUCK ANIMATE
            bool play_cling = puck.animate(elapsed,strikers);

            //REFLECTION SOUND
            if(play_cling)
            {
                cling.play();
                cling.setPlayingOffset(sf::seconds(0.f));

            }
            //GOAL 0-no goal, 1-player1 scores, 2-player2 scores
            int goal = puck.check_goal();
            if(goal==1 || goal==2)
            {
                goal_sound.play();
                goal_sound.setPlayingOffset(sf::seconds(0.f));

                player_blue.get_striker()->setPosition(280,120);
                player_red.get_striker()->setPosition(280,710);

                if(goal==1)
                {
                    player_blue.add_point();
                }
                if(goal==2)
                {
                    player_red.add_point();
                }
            }

            if(((player_blue.get_points()==points_limit || player_red.get_points()==points_limit) && points_limit>0)
                    || (time_limit>0 && remained_min==0 && remained_sec==0))
            {
                match=false;
                end_of_match=true;
                music_after_match.play();
            }


            window.draw(result_text);
            window.draw(puck);
            window.draw(*player_blue.get_striker());
            window.draw(*player_blue.get_striker_internal());
            window.draw(*player_red.get_striker());
            window.draw(*player_red.get_striker_internal());
        }

        //END OF MATCH
        if(end_of_match)
        {
            std::string text = "CONGRATULATIONS!\n\n";

            if(player_blue.get_points()==points_limit && points_limit>0)
            {
                text+=" BLUE PLAYER WINS\n\t\t\t   "+std::to_string(player_blue.get_points())+"-"+std::to_string(player_red.get_points());
            }
            else if(player_red.get_points()==points_limit && points_limit>0)
            {
                text+=" RED PLAYER WINS\n\t\t\t   "+std::to_string(player_red.get_points())+"-"+std::to_string(player_blue.get_points());
            }

            if(time_limit>0)
            {
                if(player_blue.get_points() > player_red.get_points())
                {
                    text+=" BLUE PLAYER WINS\n\t\t\t   "+std::to_string(player_blue.get_points())+"-"+std::to_string(player_red.get_points());
                }
                else if(player_blue.get_points() < player_red.get_points())
                {
                    text+=" RED PLAYER WINS\n\t\t\t   "+std::to_string(player_red.get_points())+"-"+std::to_string(player_blue.get_points());
                }
                else
                {
                    text+=" DRAW\t\t\t   "+std::to_string(player_red.get_points())+"-"+std::to_string(player_blue.get_points());
                }
            }

            std::string buttons_string = "NEW MATCH\n\n     CLOSE";

            sf::Text summary = print(ttf,text,45,sf::Color::Black,sf::Vector2f(80,160));
            sf::Text buttons_text = print(ttf,buttons_string,25,sf::Color::White,sf::Vector2f(205,495));

            for(const auto &el : end_buttons)
            {
                window.draw(el);
            }
            window.draw(summary);
            window.draw(buttons_text);
        }
        window.display();
    }
}
