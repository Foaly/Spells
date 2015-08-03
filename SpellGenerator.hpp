//
// Created by Maximilian on 21.06.2015.
//

#ifndef SPELLS_SPELLGENERATOR_H
#define SPELLS_SPELLGENERATOR_H


#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <Thor/Resources/ResourceHolder.hpp>

class SpellGenerator
{
public:
    SpellGenerator(sf::Vector2f center, thor::ResourceHolder<sf::Texture, std::string>& texture);

    std::vector<sf::Sprite> generateSpirale();

    std::vector<sf::Sprite> generateWave();

private:

    sf::Sprite createCircle(sf::Vector2f position) const;

    sf::Vector2f m_center;
    thor::ResourceHolder<sf::Texture, std::string>& m_textures;
};


#endif //SPELLS_SPELLGENERATOR_H
