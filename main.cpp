#include "player.h"
#include "striker.h"
#include "puck.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(630+50, 840), "AIR HOCKEY");
    window.setVerticalSyncEnabled(true); //limit klatek
    window.setActive();

    sf::Texture texture_table;
    if (!texture_table.loadFromFile("resources/images/table.png")) {
        std::cerr << "Could not load texture of table" << std::endl;
        return 1;
    }

    sf::Music cling;
    if (!cling.openFromFile("resources/sounds/cling.wav"))
    {
        std::cerr << "Cling not load" << std::endl;
    }

    sf::Music goal_sound;
    if (!goal_sound.openFromFile("resources/sounds/goal.wav"))
    {
        std::cerr << "Goal sound not load" << std::endl;
    }

    //FONT
    sf::Font ttf;
    ttf.loadFromFile("resources/fonts/ariendezze.ttf");
    std::string s("");
    sf::Text txt(s,ttf);
    txt.setCharacterSize(120);
    txt.setFillColor(sf::Color::Black);
    txt.setPosition(525+40,300);

    std::stringstream ss;

    //TABLE
    sf::Sprite sprite_table;
    sprite_table.setTexture(texture_table);
    sprite_table.setTexture(texture_table);
    sprite_table.setScale(1.75, 1.75);

    //PLAYERS - constructor
    Player player1(280,120,sf::Color::Blue,75,365);
    Player player2(280,710,sf::Color::Red,480,770);

    //PUCK
    Puck puck(30,sf::Vector2f(280,425));
    puck.setFillColor(sf::Color(255,140,0));
    puck.setOrigin(30,30);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time time = clock.restart();

        //sf::Vector2i position = sf::Mouse::getPosition();
        //std::cout << position.x << " " << position.y << std::endl;

        //PLAYERS MOVE
        player1.get_striker()->set_previous_position(player1.get_striker()->getPosition());
        player2.get_striker()->set_previous_position(player2.get_striker()->getPosition());

        player1.Player_animate(time,0,puck.getPosition());
        player2.Player_animate(time,1,puck.getPosition());
        window.clear(sf::Color::White);

        //RESULT
        ss.str("");
        ss<< player1.get_points() << "\n" << player2.get_points();
        txt.setString(ss.str());

        std::vector<Striker> strikers={*player1.get_striker(),*player2.get_striker()};

        //PUCK ANIMATE
        bool play_cling = puck.animate(time,strikers);

        //SOUND
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

            player1.get_striker()->setPosition(280,120);
            player2.get_striker()->setPosition(280,710);

            if(goal==1)
            {
                player1.add_point();
            }
            if(goal==2)
            {
                player2.add_point();
            }
        }

        window.draw(sprite_table);
        window.draw(puck);
        window.draw(txt);
        window.draw(*player1.get_striker());
        window.draw(*player1.get_striker_internal());
        window.draw(*player2.get_striker());
        window.draw(*player2.get_striker_internal());


        window.display();
    }
}
