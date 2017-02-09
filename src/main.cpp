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

#include "Spells.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    try
    {
        Spells spellsApp;
        return spellsApp.run();
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
}

/*#include <bitset>

int main()
{
    sf::Image image;
    sf::Color color(1, 2, 3, 4);
    sf::Uint32 i = color.toInteger();
    sf::Uint32 f = ((i) >> 24) | ((i & 0x00ff0000) >> 8) | ((i & 0x0000ff00) << 8) | ((i) << 24);

    image.create(10, 10, color);
    return 0;
}*/
