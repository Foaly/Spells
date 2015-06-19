//
// Created by Maximilian on 19.06.2015.
//

#ifndef SPELLS_SPELLS_H
#define SPELLS_SPELLS_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <vector>


class Spells {
public:
    Spells();

    int run();

private:

    void handleEvents();
    void update();
    void draw();

    bool m_isUserDrawing;
    sf::RenderWindow m_window;
    std::vector<sf::CircleShape> m_spellPoints;
    std::vector<sf::CircleShape> m_userPoints;
    sf::Vector2f m_lastPosition;
};


#endif //SPELLS_SPELLS_H
