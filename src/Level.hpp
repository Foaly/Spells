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

#ifndef SPELLS_LEVEL_H
#define SPELLS_LEVEL_H

#include <string>

#include <SFML/Graphics/Color.hpp>

class Level
{
public:
    Level();

    bool loadFromFile(std::string& filename);

    std::string m_name;
    std::string m_svgPath;
    std::string m_backgroundTextureName;
    std::string m_emitterName;
    std::string m_emitterTexture;
    int         m_particleDownsampleRate;
    sf::Color   m_spellColor;
};

#endif // SPELLS_LEVEL_H
