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

#include "Level.hpp"

#include "SettingsParser/SettingsParser.hpp"
#include "Util.hpp"

#include <iostream>

Level::Level()
{

}

bool Level::loadFromFile(std::string& filename)
{
    SettingsParser spell;
    if(!spell.loadFromFile(filename))
    {
        std::cout << "Error loading settings file :" << filename << std::endl;
        return false;
    }

    std::string name;
    spell.get("name", name);
    m_name = name;

    std::string path;
    spell.get("path", path);
    m_svgPath = path;

    std::string backgroundTextureName;
    spell.get("backgroundTextureName", backgroundTextureName);
    m_backgroundTextureName = backgroundTextureName;
    
    std::string emitterName;
    spell.get("emitter", emitterName);
    m_emitterName = emitterName;
    
    std::string emitterTexture;
    spell.get("emitterTexture", emitterTexture);
    m_emitterTexture = emitterTexture;
    
    int particleDownsampleFactor = 1;
    spell.get("particleDownsampleFactor", particleDownsampleFactor);
    m_particleDownsampleFactor = particleDownsampleFactor;
    
    sf::Color defaultSpellColor(165, 0, 0, 200); // slightly transparent red
    std::vector<int> spellColor;
    spell.get("spellColor", spellColor);
    
    if (spellColor.size() == 4)
    {
        defaultSpellColor.r = clamp(spellColor[0], 0, 255);
        defaultSpellColor.g = clamp(spellColor[1], 0, 255);
        defaultSpellColor.b = clamp(spellColor[2], 0, 255);
        defaultSpellColor.a = clamp(spellColor[3], 0, 255);
    }
    m_spellColor = defaultSpellColor;

    return true;
}
