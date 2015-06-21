//
// Created by Maximilian on 19.06.2015.
//
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Spells.h"

Spells::Spells() : m_isUserDrawing(false),
                   m_startComputing(false),
                   m_window(sf::VideoMode(1024, 798), "Spells")
{
    // fill the spell point vector
    for(int i = 0; i < 20; i++)
    {
        sf::CircleShape circle;
        circle.setRadius(25);
        circle.setPosition(200 + 20 * i, 300);
        circle.setFillColor(sf::Color(255, 255, 255, 100));

        m_spellPoints.push_back(circle);
    }
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
        std::cout << "Hits: " << numberOfPointsHit << " Percent: " <<
                static_cast<float>(numberOfPointsHit) / m_spellPoints.size() * 100.f << std::endl;

        m_startComputing = false;
    }
}


void Spells::draw()
{
    // clear the m_window with black color
    m_window.clear(sf::Color::Black);

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
