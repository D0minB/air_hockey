#include "player.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(480+300, 720), "AIR HOCKEY");
    window.setVerticalSyncEnabled(true); //limit klatek

    sf::Texture texture_table;
    if (!texture_table.loadFromFile("images/table.png")) {
        std::cerr << "Could not load texture of table" << std::endl;
        return 1;
    }

    sf::Font ttf;
    ttf.loadFromFile("fonts/ariendezze.ttf");
    std::string s("");
    sf::Text txt(s,ttf);
    txt.setCharacterSize(40);
    txt.setFillColor(sf::Color::Black);
    txt.setPosition(525,50);

    sf::Text txt_big(s,ttf);
    txt_big.setCharacterSize(80);
    txt_big.setFillColor(sf::Color::Black);
    txt_big.setPosition(595,100);

    std::stringstream ss;

    sf::Sprite sprite_table;
    sprite_table.setTexture(texture_table);

    Player player1;
    Player player2;

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        sprite_table.setTexture(texture_table);
        sprite_table.setScale(1.5, 1.5);

        window.draw(sprite_table);

        ss.str("");
        ss << "PLAYER 1\n\n\n\n\n\nPLAYER 2";
        txt.setString(ss.str());
        window.draw(txt);

        ss.str("");
        ss<< player1.get_points() << "\n" << player2.get_points();
        txt_big.setString(ss.str());
        window.draw(txt_big);

        // end the current frame
        window.display();
    }

    return 0;
}
