#include "player.h"
#include "striker.h"
#include "puck.h"

sf::Text print(const sf::Font &ttf,const std::string& s,const int &size, const sf::Color &color, const sf::Vector2f &position)
{
   std::stringstream ss;

    sf::Text text(s,ttf);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(position);

    return text;
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
        sf::Sprite button;
        button.setTexture(texture_button);
        button.setScale(1,0.5);
        button.setPosition(155,490+i*55);
        end_buttons.emplace_back(button);
    }

    std::vector<sf::Sprite> menu_buttons;
    for(int i=0; i<1; i++)
    {
        sf::Sprite button;
        button.setTexture(texture_button);
        button.setScale(1,0.5);
        button.setPosition(165,450+i*55);
        menu_buttons.emplace_back(button);
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
    Puck puck(30,sf::Vector2f(280+220,425));

    sf::Clock clock;

    bool menu=1;
    bool match=0;
    bool end_of_match = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if(menu)
            {
                sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

                //BUTTON "START"
                if(menu_buttons[0].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    menu=false;
                    match=true;
                }
            }

            if(end_of_match)
            {
                sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

                //BUTTON "NEW GAME"
                if(end_buttons[0].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    puck.reset(sf::Vector2f(280+220,425));
                    player_blue.reset(sf::Vector2f(280,120));
                    player_red.reset(sf::Vector2f(280,710));
                    end_of_match=false;
                    match=true;

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
            sf::Text start_text = print(ttf,"START",25,sf::Color::White,sf::Vector2f(240,455));

            for(const auto &el : menu_buttons)
            {
                window.draw(el);
            }

            window.draw(title);
            window.draw(start_text);
        }

        //MATCH
        if(match)
        {
            //PLAYERS MOVE
            player_blue.get_striker()->set_previous_position(player_blue.get_striker()->getPosition());
            player_red.get_striker()->set_previous_position(player_red.get_striker()->getPosition());

            player_blue.Player_animate(elapsed,0,puck.getPosition());
            player_red.Player_animate(elapsed,1,puck.getPosition());

            std::vector<Striker> strikers={*player_blue.get_striker(),*player_red.get_striker()};

            //DISPLAY RESULT
            std::string result = std::to_string(player_blue.get_points())+"\n"+std::to_string(player_red.get_points());
            sf::Text result_text = print(ttf,result,120,sf::Color::Black,sf::Vector2f(470,280));

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

            if(player_blue.get_points()==7 || player_red.get_points()==7)
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

            if(player_blue.get_points()==7)
            {
                text+=" BLUE PLAYER WINS\n\t\t\t   "+std::to_string(player_blue.get_points())+"-"+std::to_string(player_red.get_points());
            }
            else
            {
                text+=" RED PLAYER WINS\n\t\t\t   "+std::to_string(player_red.get_points())+"-"+std::to_string(player_blue.get_points());
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
