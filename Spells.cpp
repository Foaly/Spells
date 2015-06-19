//
// Created by Maximilian on 19.06.2015.
//

#include <SFML/Window/Event.hpp>

#include "Spells.h"

Spells::Spells() : m_isUserDrawing(false),
                   m_window(sf::VideoMode(1024, 798), "Spells")
{
    for(int i = 0; i < 20; i++)
    {
        sf::CircleShape circle;
        circle.setRadius(40);
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
    // check all the m_window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // "close requested" event: we close the m_window
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

                sf::CircleShape circle;
                circle.setRadius(20);
                circle.setPosition(m_lastPosition - sf::Vector2f(20, 20));
                circle.setFillColor(sf::Color(165, 0, 0, 200));
                m_userPoints.push_back(circle);
            }
        }

        else if(event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // the user has stopped drawing
                m_isUserDrawing = false;
            }
        }
    }
}


void Spells::update()
{
    if(m_isUserDrawing)
    {
        const sf::Vector2f mousePosition = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
        const float dx = m_lastPosition.x - mousePosition.x;
        const float dy = m_lastPosition.y - mousePosition.y;
        const float threshold = 20.f;
        if((dx * dx + dy * dy) > threshold * threshold)
        {
            m_lastPosition = mousePosition;

            sf::CircleShape circle;
            circle.setRadius(20);
            circle.setPosition(m_lastPosition - sf::Vector2f(20, 20));
            circle.setFillColor(sf::Color(165, 0, 0, 200));
            m_userPoints.push_back(circle);
        }

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

