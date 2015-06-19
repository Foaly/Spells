//
// Created by Maximilian on 19.06.2015.
//

#ifndef SPELLS_SPELLS_H
#define SPELLS_SPELLS_H

#include <SFML/Graphics/RenderWindow.hpp>


class Spells {
public:
    Spells();

    int run();

private:

    void handleEvents();
    void update();
    void draw();

    sf::RenderWindow window;

};


#endif //SPELLS_SPELLS_H
