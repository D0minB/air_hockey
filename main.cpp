#include "player.h"
#include "striker.h"
#include "puck.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(630+50, 840), "AIR HOCKEY");
    window.setVerticalSyncEnabled(true); //frame limit

    sf::Texture texture_table;
    if (!texture_table.loadFromFile("resources/images/table.png")) {
        std::cerr << "Could not load texture of table" << std::endl;
        return 1;
    }

    sf::Texture texture_button;
    if (!texture_button.loadFromFile("resources/images/button.png")) {
        std::cerr << "Could not load texture of button" << std::endl;
        return 1;
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

    sf::Music after_match;
    if (!after_match.openFromFile("resources/sounds/safe_and_sound.wav"))
    {
        std::cerr << "Music after match not load" << std::endl;
        return 1;
    }
    after_match.setPlayingOffset(sf::seconds(2.f));

    //FONT
    sf::Font ttf;
    if(!ttf.loadFromFile("resources/fonts/Roboto-Black.ttf"))
    {
        std::cerr << "Font not load" << std::endl;
        return 1;
    }

    std::string s("");
    sf::Text txt(s,ttf);
    txt.setCharacterSize(120);
    txt.setFillColor(sf::Color::Black);
    txt.setPosition(470,280);

    sf::Text end_txt(s,ttf);
    end_txt.setCharacterSize(45);
    end_txt.setFillColor(sf::Color::Black);
    end_txt.setPosition(80,160);

    sf::Text button_txt(s,ttf);
    button_txt.setCharacterSize(25);
    button_txt.setFillColor(sf::Color::White);
    button_txt.setPosition(205,495);

    std::stringstream ss;

    //TABLE
    sf::Sprite sprite_table;
    sprite_table.setTexture(texture_table);
    sprite_table.setScale(1.75, 1.75);

    //BUTTON
    std::vector<sf::Sprite> end_buttons;

    for(int i=0; i<2; i++)
    {
        sf::Sprite button;
        button.setTexture(texture_button);
        button.setScale(1,0.5);
        button.setPosition(155,490+i*55);
        end_buttons.emplace_back(button);
    }


    //PLAYERS - constructor
    Player player_blue(280,120,sf::Color::Blue,75,365);
    Player player_red(280,710,sf::Color::Red,480,770);

    //PUCK
    Puck puck(30,sf::Vector2f(280+220,425));
    puck.setFillColor(sf::Color(255,140,0));
    puck.setOrigin(puck.getRadius(),puck.getRadius());

    sf::Clock clock;



    bool end_of_match = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if(end_of_match)
            {
                sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
                //BUTTON "CLOSE"
                if(end_buttons[1].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    window.close();
                }

                //BUTTON "NEW GAME"
                if(end_buttons[0].getGlobalBounds().contains(mouse_position.x,mouse_position.y) && event.mouseButton.button == sf::Mouse::Left)
                {
                    puck.reset(sf::Vector2f(280+220,425));
                    player_blue.reset(sf::Vector2f(280,120));
                    player_red.reset(sf::Vector2f(280,710));
                    end_of_match=false;
                }
            }

        }

        sf::Time elapsed = clock.restart();

        window.clear(sf::Color::White);
        window.draw(sprite_table);


        if(!end_of_match)
        {
            after_match.stop();
            after_match.setPlayingOffset(sf::seconds(15.f));


            //PLAYERS MOVE
            player_blue.get_striker()->set_previous_position(player_blue.get_striker()->getPosition());
            player_red.get_striker()->set_previous_position(player_red.get_striker()->getPosition());

            player_blue.Player_animate(elapsed,0,puck.getPosition());
            player_red.Player_animate(elapsed,1,puck.getPosition());

            std::vector<Striker> strikers={*player_blue.get_striker(),*player_red.get_striker()};

            //RESULT
            ss.str("");
            ss<< player_blue.get_points() << "\n" << player_red.get_points();
            txt.setString(ss.str());

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
                end_of_match=true;
                after_match.play();
            }

            window.draw(puck);
            window.draw(txt);
            window.draw(*player_blue.get_striker());
            window.draw(*player_blue.get_striker_internal());
            window.draw(*player_red.get_striker());
            window.draw(*player_red.get_striker_internal());
        }

        //END OF MATCH
        else
        {
            ss.str("");
            ss << "CONGRATULATIONS!\n\n";

            if(player_blue.get_points()==7)
            {
                ss << " BLUE PLAYER WINS\n\t\t\t   "<< player_blue.get_points() << "-" << player_red.get_points();
            }
            else
            {
                ss << " RED PLAYER WINS\n\t\t\t   " << player_red.get_points() << "-" << player_blue.get_points();
            }
            end_txt.setString(ss.str());

            ss.str("");
            ss.clear();
            ss << "NEW MATCH\n\n     CLOSE";
            button_txt.setString(ss.str());

            window.draw(end_txt);

            for(const auto &el : end_buttons)
            {
                window.draw(el);
            }
            window.draw(button_txt);
        }
        window.display();
    }
}
