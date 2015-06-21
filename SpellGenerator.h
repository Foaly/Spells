//
// Created by Maximilian on 21.06.2015.
//

#ifndef SPELLS_SPELLGENERATOR_H
#define SPELLS_SPELLGENERATOR_H


#include <vector>
#include <SFML/Graphics/CircleShape.hpp>

class SpellGenerator
{
public:
    SpellGenerator(sf::Vector2f center);

    std::vector<sf::CircleShape> generateSpirale();

private:

    sf::CircleShape createCircle(sf::Vector2f position) const;

    sf::Vector2f m_center;
};


#endif //SPELLS_SPELLGENERATOR_H
