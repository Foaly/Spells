//
// Created by Maximilian on 19.06.2015.
//

#ifndef SPELLS_SPELLS_H
#define SPELLS_SPELLS_H

#include <vector>

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

class Spells {
public:
    Spells();

    int run();

private:

    void handleEvents();
    void update();
    void draw();

    bool m_isUserDrawing;
    bool m_isComputing;

    sf::Clock m_frameClock;
    sf::RenderWindow m_window;
    const sf::Vector2f m_windowCenter;

    thor::ResourceHolder<sf::Texture, std::string> m_textures;
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
    
    sf::SoundBuffer m_failSoundBuffer;
    sf::Sound m_failSound;
    thor::ParticleSystem m_failParticleSystem;
    std::vector<sf::Vector2f> m_failedPoints;

    sf::Vector2f m_lastPosition;

    thor::ParticleSystem m_winParticleSystem;
    
    sf::Sprite m_backgroundSprite;
    sf::RectangleShape m_overlayRect;

    sf::Font m_font;
    sf::Text m_percentageText;

    sf::Shader m_radialGradientShader;
    sf::Shader m_rectangleGradientShader;
};


#endif //SPELLS_SPELLS_H
