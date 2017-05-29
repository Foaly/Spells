/// Spells - a game about magic spells
/// Copyright (C) 2015 - 2017  Foaly

/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.

/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.

/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Created by Maximilian on 19.06.2015.


#ifndef SPELLS_SPELLS_H
#define SPELLS_SPELLS_H

#include <vector>
#include <string>
#include <map>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <Thor/Particles.hpp>
#include <Thor/Resources/ResourceHolder.hpp>

#include "SpellGenerator.hpp"
#include "FallingPointEmitter.hpp"
#include "Level.hpp"
#include "LoadHelpers.hpp"

class Spells {
public:
    Spells();

    int run();

private:

    void handleEvents();
    void update();
    void draw();
    void loadSpells(std::string spellsFileDirectory);
    void setSpell(Level& spell);

    bool m_isUserDrawing;
    bool m_isComputing;

    sf::Clock m_frameClock;
    sf::RenderWindow m_window;
    const sf::Vector2f m_windowCenter;

    thor::ResourceHolder<sf::Texture, std::string> m_textures;
    thor::ResourceHolder<sf::SoundBuffer, std::string> m_sounds;
    const float m_userPointRadius;
    SpellGenerator m_spellGenerator;
    std::vector<sf::Vector2f> m_spellPoints;
    std::vector<sf::Vector2f> m_userPoints;
    std::vector<sf::Vector2f>::iterator m_userPointIter;
    std::vector<sf::Vector2f> m_spellPointsCopy;
    unsigned int m_numberOfPointsHit;
    sf::Clock m_computingClock;
    thor::ParticleSystem m_fallingPointParticleSystem;
    FallingPointEmitter m_fallingPointEmitter;

    sf::Sound m_winSound;
    sf::Sound m_failSound;
    thor::ParticleSystem m_failParticleSystem;
    std::vector<sf::Vector2f> m_failedPoints;

    sf::Vector2f m_lastPosition;

    thor::ParticleSystem m_winParticleSystem;
    std::vector<sf::Vector2f> m_winPoints;

    sf::Sprite m_backgroundSprite;
    sf::RectangleShape m_overlayRect;
    sf::Sprite m_wand;
    thor::ParticleSystem m_wandParticles;
    thor::UniversalEmitter m_wandEmitter;

    std::map<std::string, Level> m_level;
    std::map<std::string, Level>::iterator m_currentSpell;
    EmitterMap m_emitters;
    AffectorMap m_affectors;

    sf::Font m_font;
    sf::Text m_percentageText;
    sf::Sprite m_percentBackground;

    sf::Shader m_radialGradientShader;
    sf::Shader m_rectangleGradientShader;
};


#endif //SPELLS_SPELLS_H
