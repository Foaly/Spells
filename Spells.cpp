//
// Created by Maximilian on 19.06.2015.
//
#include <iostream>
#include <sstream>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Thor/Math.hpp>

#include "Spells.hpp"
#include "Util.hpp"
#include "VectorEmitter.hpp"

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
                   m_startComputing(false),
                   m_window(sf::VideoMode(1024, 798), "Spells"),
                   m_windowCenter(m_window.getSize().x / 2, m_window.getSize().y / 2),
                   m_spellGenerator(sf::Vector2f(m_window.getSize().x / 2 - 25, m_window.getSize().y / 2 - 25))
{
    //m_spellPoints = m_spellGenerator.generateSpirale();
    auto wave = m_spellGenerator.generateWave();
    std::copy(wave.begin(), wave.end(), back_inserter(m_spellPoints));

    // limit frame time
    m_window.setFramerateLimit(60);

    if(!m_particleTexture.loadFromFile("data/old key small.png"))
    {
        std::cerr << "Failed to load particle texture!" << std::endl;
    }

    // set particle texture
    m_particleSystem.setTexture(m_particleTexture);

    if(!m_font.loadFromFile("data/fonts/BilboSwashCaps-Regular.otf"))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    m_percentageText.setFont(m_font);
    m_percentageText.setString("0%");
    m_percentageText.setCharacterSize(50);
    m_percentageText.setPosition(30, 10);
    m_percentageText.setStyle(sf::Text::Style::Bold);
    // TODO horizontal character spacing

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
                    break;
                default:
                    break;
            }
        }

        else if(event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // the user starts drawing
                m_isUserDrawing = true;
                const sf::Vector2i pixelPosition(event.mouseButton.x, event.mouseButton.y);
                m_lastPosition = m_window.mapPixelToCoords(pixelPosition);

                addUserPoint(m_lastPosition);
            }
        }

        else if(event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // the user has stopped drawing
                m_isUserDrawing = false;
                m_startComputing = true;
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
        const sf::Vector2f delta(m_lastPosition - mousePosition);
        const float threshold = 20.f;

        // if the mouse has moved further away form the
        // last position than the threshold add a new point
        if((delta.x * delta.x + delta.y * delta.y) > threshold * threshold)
        {
            m_lastPosition = mousePosition;
            addUserPoint(m_lastPosition);
        }
    }
    else if(m_startComputing)
    {
        unsigned int numberOfPointsHit = 0;
        auto spellPoints = m_spellPoints; // make a copy of the spell points

        for(auto userPoint: m_userPoints)
        {
            // check if the user point is within range of a spell point, if so
            // increment the counter and remove the spell point (every spell point can only be hit once)
            for(auto itor = spellPoints.begin(); itor != spellPoints.end();)
            {
                const sf::Vector2f spellPointPosition(itor->getPosition());
                const sf::Vector2f userPointPosition(userPoint.getPosition());
                const sf::Vector2f delta(spellPointPosition - userPointPosition);

                if((delta.x * delta.x + delta.y * delta.y) < 20 * 20)
                {
                    numberOfPointsHit++;
                    spellPoints.erase(itor);
                    break;
                }
                else
                {
                    itor++;
                }
            }
        }

        // calculate the percentage of points hit
        const float percent = std::round(static_cast<float>(numberOfPointsHit) / m_spellPoints.size() * 100.f);
        std::cout << "Hits: " << numberOfPointsHit << " Percent: " << percent << std::endl;

        // TODO: this should use std::to_string. This is a compiler error an update should help
        m_percentageText.setString(toString(percent) + "%");

        // if more than 70% are covered play an animation
        if(percent > 70.f)
        {
            VectorEmitter emitter(m_spellPoints);
            emitter.setEmissionRate(10);
            emitter.setParticleLifetime( thor::Distributions::uniform(sf::seconds(1.2f), sf::seconds(1.6f)) );
            emitter.setParticlePosition( thor::Distributions::circle(m_windowCenter, 50) );   // Emit particles in a circle 50px around the center
            emitter.setParticleVelocity( util::Distributions::disk(sf::Vector2f(), 100.f, 200.f) );   // Emit particles with a velocity between 100.f and 200.f in a random direction
            emitter.setParticleRotation( thor::Distributions::uniform(0.f, 360.f) );      // Rotate randomly
            emitter.setParticleRotationSpeed( thor::Distributions::uniform(10.f, 50.f));  // random rotation speed
            m_particleSystem.addEmitter(emitter, sf::seconds(2.f));
        }

        m_startComputing = false;
    }

    // update particle system
    m_particleSystem.update(frameTime);
}


void Spells::draw()
{
    // clear the m_window with black color
    m_window.clear(sf::Color::Black);

    // draw particle system
    m_window.draw(m_particleSystem);

    // draw the spell
    for(auto circle: m_spellPoints)
    {
        m_window.draw(circle);
    }

    // draw the user spell
    for(auto circle: m_userPoints)
    {
        m_window.draw(circle);
    }

    // draw percentage text
    m_window.draw(m_percentageText);

    // end the current frame
    m_window.display();
}

/**
 * @brief: Adds a circle at the given point to the user points vector.
 */
void Spells::addUserPoint(const sf::Vector2f &point)
{
    sf::CircleShape circle;
    circle.setRadius(20);
    circle.setPosition(point - sf::Vector2f(20, 20));
    circle.setFillColor(sf::Color(165, 0, 0, 200));
    m_userPoints.push_back(circle);
}
