#include "player.h"
#include "striker.h"
#include "puck.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(630+200, 840), "AIR HOCKEY");
    window.setVerticalSyncEnabled(true); //limit klatek

    sf::Texture texture_table;
    if (!texture_table.loadFromFile("resources/images/table.png")) {
        std::cerr << "Could not load texture of table" << std::endl;
        return 1;
    }

    sf::Music cling;
    if (!cling.openFromFile("resources/sounds/cling.wav"))
    {
        std::cerr << "Sound not load" << std::endl;
    }

    //FONT
    sf::Font ttf;
    ttf.loadFromFile("resources/fonts/ariendezze.ttf");
    std::string s("");
    sf::Text txt(s,ttf);
    txt.setCharacterSize(40);
    txt.setFillColor(sf::Color::Black);
    txt.setPosition(455+150,50);

    sf::Text txt_big(s,ttf);
    txt_big.setCharacterSize(80);
    txt_big.setFillColor(sf::Color::Black);
    txt_big.setPosition(525+150,100);

    std::stringstream ss;

    //TABLE
    sf::Sprite sprite_table;
    sprite_table.setTexture(texture_table);

    //PLAYERS - constructor
    Player player1(280,120,sf::Color::Blue,75,365);
    Player player2(280,710,sf::Color::Red,480,770);

    //PUCK
    Puck puck(30,sf::Vector2f(280,425),400,400);
    puck.setFillColor(sf::Color(255,140,0));
    puck.setOutlineThickness(5);
    puck.setOutlineColor(sf::Color(255,140,0));
    puck.setOrigin(30,30);

    sf::Clock clock;

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time time = clock.restart();

        //PLAYERS MOVE
        player1.Player_animate(time,0);
        player2.Player_animate(time,1);
        //        puck.animate(time,0);
        //        std::cout << puck.getPosition().y << std::endl;

        window.clear(sf::Color::White);


        sprite_table.setTexture(texture_table);
        sprite_table.setScale(1.75, 1.75);
        window.draw(sprite_table);

        //TEXT
        ss.str("");
        ss << "PLAYER 1\n\n\n\n\n\nPLAYER 2";
        txt.setString(ss.str());
        window.draw(txt);

        ss.str("");
        ss<< player1.get_points() << "\n" << player2.get_points();
        txt_big.setString(ss.str());
        window.draw(txt_big);

        std::vector<Striker> strikers={*player1.get_striker(),*player2.get_striker()};
        bool play_cling=puck.animate(time,strikers);

        if(play_cling)
        {
            cling.play();
            cling.setPlayingOffset(sf::seconds(0.f));

        }
        window.draw(puck);

        if(puck.getPosition().x>195 && puck.getPosition().x<375)
        {
            if(puck.getPosition().y>785)
            {
                player1.add_point();
            }
            else if(puck.getPosition().y<55)
            {
                player2.add_point();
            }
        }

        window.draw(*player1.get_striker());
        window.draw(*player1.get_striker_internal());
        window.draw(*player2.get_striker());
        window.draw(*player2.get_striker_internal());

        window.display();
    }
}
