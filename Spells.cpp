//
// Created by Maximilian on 19.06.2015.
//

#include <SFML/Window/Event.hpp>

#include "Spells.h"

Spells::Spells() : window(sf::VideoMode(1024, 798), "Spells")
{

}


int Spells::run()
{
    // run the program as long as the window is open
    while (window.isOpen())
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
    while (window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            window.close();

        else if(event.type == sf::Event::KeyReleased)
        {
            switch(event.key.code)
            {
                // close if escape key was pressed
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                default:
                    break;
            }
        }
    }
}


void Spells::update()
{

}


void Spells::draw()
{
    // clear the window with black color
    window.clear(sf::Color::Black);

    // draw everything here...
    // window.draw(...);

    // end the current frame
    window.display();
}

