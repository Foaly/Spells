//
// Created by Maximilian on 19.06.2015.
//
#include <iostream>
#include <sstream>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Thor/Math.hpp>
#include <Thor/Resources/SfmlLoaders.hpp>

#include "Spells.hpp"
#include "Util.hpp"
#include "VectorEmitter.hpp"
#include "PathResolver.hpp"

namespace
{
    template <typename T>
    std::string toString ( T Number )
    {
        std::stringstream ss;
        ss << Number;
        return ss.str();
    }
}

Spells::Spells() : m_isUserDrawing(false),
                   m_isComputing(false),
                   m_window(sf::VideoMode(1024, 798), "Spells"),
                   m_windowCenter(m_window.getSize().x / 2, m_window.getSize().y / 2),
                   m_userPointRadius(20.f),
                   m_spellGenerator(m_windowCenter)
{
    // limit frame time
    m_window.setFramerateLimit(60);

    try
    {
        m_textures.acquire("circle", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/Circle.png")), thor::Resources::Reuse);
        m_textures.acquire("key", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/old key small.png")), thor::Resources::Reuse);
    }
    catch (thor::ResourceLoadingException &e)
    {
        std::cout << e.what() << std::endl;
        // TODO: better error handeling, program will crash on texture access, maybe shutdown gracefully
    }

    // set particle texture
    m_particleSystem.setTexture(m_textures["key"]);

    m_fallingPointParticleSystem.setTexture(m_textures["circle"]);
    m_fallingPointParticleSystem.addEmitter(thor::refEmitter(m_fallingPointEmitter));

    if(!m_font.loadFromFile(resolvePath("data/fonts/BilboSwashCaps-Regular.otf")))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    m_percentageText.setFont(m_font);
    m_percentageText.setString("0%");
    m_percentageText.setCharacterSize(50);
    m_percentageText.setPosition(30, 10);
    m_percentageText.setStyle(sf::Text::Style::Bold);
    // TODO horizontal character spacing

    if(sf::Shader::isAvailable())
    {
        if(!m_radialGradientShader.loadFromFile(resolvePath("data/shader/RadialGradient.frag"), sf::Shader::Fragment))
        {
            std::cerr << "Failed to load radial gradient shader!" << std::endl;
        }
    }

    if(!m_failSoundBuffer.loadFromFile(resolvePath("data/sounds/fail.wav")))
    {
        std::cerr << "Failed to load the fail sound!" << std::endl;
    }

    m_failSound.setBuffer(m_failSoundBuffer);

    m_spellPoints = m_spellGenerator.generateSpirale();
    //auto wave = m_spellGenerator.generateWave();
    //std::copy(wave.begin(), wave.end(), back_inserter(m_spellPoints));


    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
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
                    m_percentageText.setString("0%");
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
                }
            }
        }
    }
}


void Spells::update()
{
    // get frame time
    const sf::Time frameTime = m_frameClock.restart();

    if(m_isUserDrawing)
    {
        const sf::Vector2f mousePosition(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
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
        if(m_computingClock.getElapsedTime() >= sf::milliseconds(30))
        {
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
            const float percent = std::round(static_cast<float>(m_numberOfPointsHit) / m_spellPoints.size() * 100.f);

            // TODO: this should use std::to_string. This is a compiler error an update should help
            m_percentageText.setString(toString(percent) + "%");

            // if we have checked all user points play an animation if enough percent are covert
            if (m_userPointIter == m_userPoints.end())
            {
                std::cout << "Hits: " << m_numberOfPointsHit << " Percent: " << percent << std::endl;

                // if more than 70% are covered play an animation
                if(percent > 70.f)
                {
                    VectorEmitter emitter(m_spellPoints);
                    emitter.setEmissionRate(10);
                    emitter.setParticleLifetime( thor::Distributions::uniform(sf::seconds(1.2f), sf::seconds(1.6f)) );
                    emitter.setParticleVelocity( util::Distributions::disk(sf::Vector2f(), 100.f, 200.f) );   // Emit particles with a velocity between 100.f and 200.f in a random direction
                    emitter.setParticleRotation( thor::Distributions::uniform(0.f, 360.f) );      // Rotate randomly
                    emitter.setParticleRotationSpeed( thor::Distributions::uniform(10.f, 50.f));  // random rotation speed
                    m_particleSystem.addEmitter(emitter, sf::seconds(2.f));
                }
                else
                {
                    m_userPoints.clear();
                    m_failSound.play();
                    m_percentageText.setString("0%");
                }

                m_isComputing = false;
            }

            m_computingClock.restart();
        }
    }

    // update particle system
    m_particleSystem.update(frameTime);
    m_fallingPointParticleSystem.update(frameTime);
}


void Spells::draw()
{
    // clear the m_window with black color
    m_window.clear(sf::Color::Black);

    // draw particle system
    m_window.draw(m_particleSystem);

    // draw the spell
    m_radialGradientShader.setUniform("radiuses", sf::Vector2f(0.5f, 0.4f));
    sf::Sprite circle(m_textures["circle"]);
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

    // draw the user spell
    circle.setScale(0.8f, 0.8f);
    circle.setColor(sf::Color(165, 0, 0, 200));
    for(auto position: m_userPoints)
    {
        circle.setPosition(position);
        m_window.draw(circle, &m_radialGradientShader);
    }

    // draw percentage text
    m_window.draw(m_percentageText);

    // end the current frame
    m_window.display();
}
