#include "Level.hpp"

#include "SettingsParser/SettingsParser.hpp"

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
    
    return true;
}
