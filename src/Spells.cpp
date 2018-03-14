/// Spells - a game about magic spells
/// Copyright (C) 2015 - 2018  Foaly

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

#include <iostream>
#include <sstream>
#include <iterator>

#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Thor/Math.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Thor/Particles/Affectors.hpp>
#include <Thor/Animations/ColorAnimation.hpp>

#include "Spells.hpp"
#include "Util.hpp"
#include "VectorEmitter.hpp"
#include "PathResolver.hpp"



Spells::Spells() : m_isUserDrawing(false),
                   m_isComputing(false),
                   m_window(sf::VideoMode(1280, 800), "Spells", sf::Style::Fullscreen, sf::ContextSettings(0, 0, 8)),
                   m_windowCenter(m_window.getSize().x / 2, m_window.getSize().y / 2),
                   m_userPointRadius(20.f),
                   m_spellGenerator(m_windowCenter),
                   m_textures(loadTextures()),    // load textures
                   m_clock(m_textures)
{
    // limit frame time
    m_window.setFramerateLimit(60);
    
    // set up overlay
    m_overlayRect.setSize(sf::Vector2f(m_window.getSize().x - 200, m_window.getSize().y - 50));
    m_overlayRect.setPosition(100, 25);
    m_overlayRect.setFillColor(sf::Color(0, 0, 0, 100));
    m_overlayRect.setTexture(&m_textures["rect.png"]);

    m_window.setMouseCursorVisible(false);
    m_textures["wand.png"].setSmooth(true);
    m_wand.setTexture(m_textures["wand.png"]);
    m_wand.setScale(-1.f, 1.f); // uncomment for left hand
    m_wand.setOrigin(94.f, 3.f);
    
    // load emitters and affectors for the particle systems
    m_emitters = setupEmitters(m_winPoints);
    m_affectors = setupAffectors();

    // set up the particle systems
    m_failParticleSystem.setTexture(m_textures["circle.png"]);

    m_fallingPointParticleSystem.setTexture(m_textures["circle.png"]);
    m_fallingPointParticleSystem.addEmitter(thor::refEmitter(m_fallingPointEmitter));

    m_textures["star.png"].setRepeated(false);
    m_textures["star.png"].setSmooth(true);
    m_wandParticles.setTexture(m_textures["star.png"]);
    m_wandParticles.addTextureRect(sf::IntRect(0, 0, 48, 48));
    m_wandParticles.addTextureRect(sf::IntRect(48, 0, 96, 48));
    m_wandEmitter.setEmissionRate(30.f);
    m_wandEmitter.setParticleLifetime( thor::Distributions::uniform(sf::seconds(0.8f), sf::seconds(1.0f)));
    m_wandEmitter.setParticlePosition( thor::Distributions::circle(sf::Vector2f(300, 300), 10) );
    m_wandEmitter.setParticleVelocity( util::Distributions::disk(60.f, 100.f) );
    m_wandEmitter.setParticleRotation( thor::Distributions::uniform(0.f, 360.f) );
    m_wandEmitter.setParticleRotationSpeed( thor::Distributions::uniform(40.f, 60.f));
    m_wandEmitter.setParticleTextureIndex( thor::Distributions::uniform(0, 1));
    m_wandEmitter.setParticleScale( util::Distributions::constant(sf::Vector2f(0.25f, 0.25f)) );
    m_wandParticles.addEmitter(thor::refEmitter(m_wandEmitter));

    m_wandParticles.addAffector(m_affectors["downwards"]);
    m_wandParticles.addAffector(m_affectors["goldToTransparent"]);

    // font loading and text setup
    if(!m_font.loadFromFile(resolvePath("data/fonts/BilboSwashCaps-Regular.otf")))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }
    
    m_percentBackground.setTexture(m_textures["parchment.png"]);
    m_percentBackground.setPosition(5, 10);

    // move clock to correct position
    const int clockXOffset = m_window.getSize().x - m_clock.getSize().x - 5;
    m_clock.setPosition(sf::Vector2f(clockXOffset, 1));

    m_percentageText.setFont(m_font);
    m_percentageText.setCharacterSize(50);
    m_percentageText.setPosition(35, 30);
    m_percentageText.setStyle(sf::Text::Style::Bold);
    // TODO horizontal character spacing

    if(sf::Shader::isAvailable())
    {
        if(!m_radialGradientShader.loadFromFile(resolvePath("data/shader/RadialGradient.frag"), sf::Shader::Fragment))
        {
            std::cerr << "Failed to load radial gradient shader!" << std::endl;
        }
        if(!m_rectangleGradientShader.loadFromFile(resolvePath("data/shader/RectangleGradient.frag"), sf::Shader::Fragment))
        {
            std::cerr << "Failed to load radial gradient shader!" << std::endl;
        }
        m_rectangleGradientShader.setUniform("leftTop", sf::Vector2f(0.f, 0.05f));
        m_rectangleGradientShader.setUniform("rightBottom", sf::Vector2f(0.95f, 1.0));
    }

    // load sounds
    m_sounds = loadSounds();
    m_failSound.setBuffer(m_sounds["fail.wav"]);

    //m_spellPoints = m_spellGenerator.generateSpirale();
    //m_spellPoints = m_spellGenerator.generateWave();

    //BezierCurve curve(sf::Vector2f(300, 400), sf::Vector2f(500, 100), sf::Vector2f(700, 700), sf::Vector2f(900, 400));
    //m_spellPoints = curve.generateEvenlySpacedPoints(20.f); // distance of 20px between points

    loadSpells(resolvePath("data/spells/"));

    m_currentSpell = m_level.begin();
    setSpell(m_currentSpell->second);


    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    std::cout << "Thor version: " << THOR_VERSION_MAJOR << "." << THOR_VERSION_MINOR << std::endl;
    std::cout << std::endl;
}


int Spells::run()
{
    // run the program as long as the m_window is open
    while (m_window.isOpen())
    {
        // handle events
        handleEvents();

        // update everything
        update();

        // draw everything
        draw();
    }

    return 0;
}



void Spells::handleEvents()
{
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // "close requested" event: close the window
        if (event.type == sf::Event::Closed)
            m_window.close();

        else if(event.type == sf::Event::KeyReleased)
        {
            switch(event.key.code)
            {
                // close if escape key was pressed
                case sf::Keyboard::Escape:
                    m_window.close();
                    break;
                // delete all the user points
                case sf::Keyboard::C:
                    m_userPoints.clear();
                    m_percentageText.setString("  0%");
                    break;
                // switch to the next spell
                case sf::Keyboard::Right:
                    if (!m_isComputing)
                    {
                        m_currentSpell++;
                        if (m_currentSpell == m_level.end())
                            m_currentSpell = m_level.begin();
                        setSpell(m_currentSpell->second);
                    }
                    break;
                // switch to the previous spell
                case sf::Keyboard::Left:
                    if (!m_isComputing)
                    {
                        if (m_currentSpell == m_level.begin())
                            m_currentSpell = std::prev(m_level.end());
                        else
                            m_currentSpell--;
                        setSpell(m_currentSpell->second);
                    }
                    break;
                default:
                    break;
            }
        }

        else if(event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if(!m_isComputing)
                {
                    // the user starts drawing
                    m_isUserDrawing = true;
                    const sf::Vector2i pixelPosition(event.mouseButton.x, event.mouseButton.y);
                    m_lastPosition = m_window.mapPixelToCoords(pixelPosition);
                    m_userPoints.push_back(m_lastPosition);
                }
            }
        }
        else if(event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if(!m_isComputing)
                {
                    // the user has stopped drawing
                    // set up everything for the calculation of the hit percentage
                    m_isUserDrawing = false;
                    m_isComputing = true;
                    m_spellPointsCopy = m_spellPoints; // make a copy of the spell points
                    m_userPointIter = m_userPoints.begin();
                    m_numberOfPointsHit = 0;
                    m_computingClock.restart();
                    m_wandParticles.clearEmitters();
                }
            }
        }
    }
}


void Spells::update()
{
    // get frame time
    const sf::Time frameTime = m_frameClock.restart();

    // get mouse position
    const sf::Vector2f mousePosition(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));

    // update wand
    m_wand.setPosition(mousePosition);
    m_wandEmitter.setParticlePosition(mousePosition);

    m_clock.update(frameTime);

    if(m_isUserDrawing)
    {
        const sf::Vector2f delta(mousePosition - m_lastPosition);
        const float movementLength = thor::length(delta);

        // add a point for every 20 pixel the mouse moves between two frames
        if(movementLength > m_userPointRadius)
        {
            const sf::Vector2f direction(thor::unitVector(delta));
            const int steps = static_cast<int>(std::ceil(movementLength)) / static_cast<int>(m_userPointRadius);
            for(int i = 0; i < steps; i++)
            {
                m_lastPosition += direction * m_userPointRadius;
                m_userPoints.push_back(m_lastPosition);
            }
        }
    }
    else if(m_isComputing)
    {
        if(m_computingClock.getElapsedTime() >= sf::milliseconds(30)) // this clock determines the speed at which the wrong points fall
        {
            if (m_userPoints.size() == 0)
            {
                m_isComputing = false;
                return;
            }

            bool didUserPointHit = false;
            const sf::Vector2f userPointPosition(*m_userPointIter);

            // check if the user point we are currently looking at is close to any spell point
            for (auto iter = m_spellPointsCopy.begin(); iter != m_spellPointsCopy.end();)
            {
                // calculate the distance between the current user point the current spell point
                const sf::Vector2f spellPointPosition(*iter);
                const sf::Vector2f delta(spellPointPosition - userPointPosition);
                const float distance = thor::length(delta);

                if (distance < 22.f)
                {
                    m_numberOfPointsHit++;
                    didUserPointHit = true;
                    m_spellPointsCopy.erase(iter);
                    break;
                }
                else
                {
                    iter++;
                }
            }

            if (didUserPointHit)
            {
                // if the user point is close to a spell point simply keep going
                m_userPointIter++;
            }
            else
            {
                // if the user point is not near any spell point remove it and make a falling point animation
                const sf::Vector2f position = *m_userPointIter;
                m_fallingPointEmitter.emitParticle(position);
                m_userPointIter = m_userPoints.erase(m_userPointIter);
            }

            // calculate the percentage of points hit
            const int percent = std::round(static_cast<float>(m_numberOfPointsHit) / m_spellPoints.size() * 100.f);

            // pad the percent string with whitespace
            auto percentString = std::to_string(percent) + "%";
            percentString.insert(0, 3 - numDigits(percent), ' ');
            m_percentageText.setString(percentString);

            // if we have checked all user points play an animation if enough percent are covert
            if (m_userPointIter == m_userPoints.end())
            {
                std::cout << "Hits: " << m_numberOfPointsHit << " Percent: " << percent << std::endl;

                // if more than 70% are covered play an animation
                if(percent > 70.f)
                {
                    int downsampleFactor = m_currentSpell->second.m_particleDownsampleFactor;
                    m_winPoints.resize(m_userPoints.size() / downsampleFactor);
                    downsample(m_userPoints.begin(), m_userPoints.end(), m_winPoints.begin(), downsampleFactor);
                    
                    m_winParticleSystem.addEmitter(m_emitters[m_currentSpell->second.m_emitterName], sf::seconds(2.f));
                    
                    for (auto& affector: m_currentSpell->second.m_affectors)
                        m_winParticleSystem.addAffector(m_affectors[affector], sf::seconds(4.f));
                    
                    m_winSound.play();
                }
                else
                {
                    // move the failed points
                    m_failedPoints = std::move(m_userPoints);

                    // generate a particle for each failed point
                    VectorEmitter emitter(m_failedPoints);
                    emitter.setEmissionRate(0);
                    emitter.setParticleLifetime( util::Distributions::constant(sf::seconds(1.8f)) );
                    m_failParticleSystem.addEmitter(emitter, sf::seconds(2.f));

                    // scale the particles up
                    m_failParticleSystem.addAffector(m_affectors["fastScaleUp"], sf::seconds(2.f));

                    // fade the points to black to transparent
                    thor::ColorGradient toBlackGradient;
                    toBlackGradient[0.0f] = m_currentSpell->second.m_spellColor; // spell color
                    toBlackGradient[0.9f] = sf::Color( 20, 20, 20,  50);         // slightly opaque grey
                    toBlackGradient[1.0f] = sf::Color(  0,  0,  0,   0);         // transparent
                    thor::ColorAnimation toBlack(toBlackGradient);
                    m_failParticleSystem.addAffector(thor::AnimationAffector(toBlack), sf::seconds(2.f));

                    // play a fail sound and reset the percent text
                    m_failSound.play();
                    m_percentageText.setString("  0%");
                }

                m_isComputing = false;
                m_wandParticles.addEmitter(thor::refEmitter(m_wandEmitter));
            }

            m_computingClock.restart();
        }
    }

    // update particle system
    m_winParticleSystem.update(frameTime);
    m_failParticleSystem.update(frameTime);
    m_fallingPointParticleSystem.update(frameTime);
    m_wandParticles.update(frameTime);
}


void Spells::draw()
{
    // clear the m_window with black color
    m_window.clear(sf::Color::Black);

    // draw background
    m_window.draw(m_backgroundSprite);

    m_window.draw(m_overlayRect, &m_rectangleGradientShader);

    // draw the win particle system
    m_window.draw(m_winParticleSystem);

    // draw the spell
    m_radialGradientShader.setUniform("radiuses", sf::Vector2f(0.5f, 0.4f));
    sf::Sprite circle(m_textures["circle.png"]);
    circle.setOrigin(25, 25);
    circle.setColor(sf::Color(255, 255, 255, 100));
    for(auto position: m_spellPoints)
    {
        circle.setPosition(position);
        m_window.draw(circle, &m_radialGradientShader);
    }

    // draw the falling user points
    m_radialGradientShader.setUniform("radiuses", sf::Vector2f(0.5f, 0.3f));
    m_window.draw(m_fallingPointParticleSystem, &m_radialGradientShader);

    // draw the fail particle system
    m_window.draw(m_failParticleSystem, &m_radialGradientShader);

    // draw the user spell
    circle.setScale(0.8f, 0.8f);
    circle.setColor(m_currentSpell->second.m_spellColor);
    for(auto position: m_userPoints)
    {
        circle.setPosition(position);
        m_window.draw(circle, &m_radialGradientShader);
    }

    // draw percentage
    m_window.draw(m_percentBackground);
    m_window.draw(m_percentageText);

    // draw clock
    m_window.draw(m_clock);

    // draw wand
    m_window.draw(m_wandParticles);
    m_window.draw(m_wand);

    // end the current frame
    m_window.display();
}


void Spells::loadSpells(std::string spellsFileDirectory)
{
    // TODO: boost filesystem: get all files in spellsFileDirectory
    // filter: get only *.svg files

    std::vector<std::string> files;
    files.push_back(resolvePath("data/spells/Alohomora.spell"));
    files.push_back(resolvePath("data/spells/Avis.spell"));
    files.push_back(resolvePath("data/spells/Lumos.spell"));
    files.push_back(resolvePath("data/spells/Wingardium_Leviosa.spell"));
    files.push_back(resolvePath("data/spells/Incendio.spell"));
    //files.push_back(resolvePath("data/spells/Dummy.spell"));

    for (auto &file: files)
    {
        Level level;
        if (!level.loadFromFile(file))
            break;

        // name and SVG path are mandatory
        if (level.m_name.empty() || level.m_svgPath.empty())
            break;

        // make sure background texture key is valid
        try {
            m_textures[level.m_backgroundTextureName].setSmooth(true); // use setter so this doesn't get optimized away. It's about the texture access.
        } catch (thor::ResourceAccessException& e) {
            level.m_backgroundTextureName = "arches.png"; // default
        }
        
        // make sure the sound names are valid
        try {
            m_sounds[level.m_sound].getChannelCount();
        } catch (thor::ResourceAccessException& e) {
            level.m_sound = "none"; // default
        }
        
        // make sure emitter texture key is valid
        try {
            m_textures[level.m_emitterTexture].setSmooth(true);
        } catch (thor::ResourceAccessException& e) {
            level.m_emitterTexture = "circle.png"; // default
        }
        
        // make sure the emitter names are valid
        if (m_emitters.find(level.m_emitterName) == m_emitters.end())
            level.m_emitterName = "circularEmitter";
        
        // remove the affectors from the level that are not valid
        for (auto affectorIter = std::begin(level.m_affectors); affectorIter != std::end(level.m_affectors); )
        {
            auto affector = trim(*affectorIter);
            if (m_affectors.find(affector) == m_affectors.end())
                affectorIter = level.m_affectors.erase(affectorIter);
            else
                affectorIter++;
        }
        
        level.m_particleDownsampleFactor = clamp(level.m_particleDownsampleFactor, 1, 10);

        m_level[level.m_name] = level;
    }
}


void Spells::setSpell(Level& spell)
{
    // reset
    m_userPoints.clear();
    m_percentageText.setString("  0%");

    // set up background
    std::string bgTextureName = spell.m_backgroundTextureName;
    m_textures[bgTextureName].setSmooth(true);
    m_backgroundSprite.setTexture(m_textures[bgTextureName]);
    sf::Vector2f scale(static_cast<float>(m_window.getSize().x) / m_textures[bgTextureName].getSize().x, static_cast<float>(m_window.getSize().y) / m_textures[bgTextureName].getSize().y);
    m_backgroundSprite.setScale(scale);
    std::cout << "Background image scale: " << scale.x << "x" << scale.y << std::endl;

    // set up path to spell SVG
    m_spellPoints = loadPathsFromFile(resolvePath("data/svg/" + spell.m_svgPath));
    
    // set the win particle system texture
    m_winParticleSystem.setTexture(m_textures[spell.m_emitterTexture]);
    m_winParticleSystem.clearParticles();
    m_winParticleSystem.clearEmitters();
    
    // set the win sound
    m_winSound.setBuffer(m_sounds[spell.m_sound]);
    
    // set up color for falling particles
    m_fallingPointEmitter.setColor(spell.m_spellColor);
}
