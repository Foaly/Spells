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
