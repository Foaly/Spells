//
// Created by Maximilian on 19.06.2015.
//

#ifndef SPELLS_SPELLS_H
#define SPELLS_SPELLS_H

#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <Thor/Particles.hpp>

#include "SpellGenerator.hpp"

class Spells {
public:
    Spells();

    int run();

private:

    void handleEvents();
    void update();
    void draw();
    void addUserPoint(const sf::Vector2f& point);

    bool m_isUserDrawing;
    bool m_startComputing;

    sf::Clock m_frameClock;
    sf::RenderWindow m_window;
    const sf::Vector2f m_windowCenter;

    const float m_userPointRadius;
    SpellGenerator m_spellGenerator;
    std::vector<sf::CircleShape> m_spellPoints;
    std::vector<sf::CircleShape> m_userPoints;

    sf::Vector2f m_lastPosition;

    thor::ParticleSystem m_particleSystem;
    sf::Texture m_particleTexture;

    sf::Font m_font;
    sf::Text m_percentageText;

    sf::Shader m_radialGradientShader;
};


#endif //SPELLS_SPELLS_H
