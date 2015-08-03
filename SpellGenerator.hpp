//
// Created by Maximilian on 21.06.2015.
//

#ifndef SPELLS_SPELLGENERATOR_H
#define SPELLS_SPELLGENERATOR_H


#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <Thor/Resources/ResourceHolder.hpp>

class SpellGenerator
{
public:
    SpellGenerator(sf::Vector2f center);

    std::vector<sf::Vector2f> generateSpirale();

    std::vector<sf::Vector2f> generateWave();

private:

    sf::Vector2f m_center;
};


#endif //SPELLS_SPELLGENERATOR_H
