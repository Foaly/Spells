#ifndef SPELLS_LEVEL_H
#define SPELLS_LEVEL_H

#include <string>

class Level
{
public:
    Level();
    
    bool loadFromFile(std::string& filename);
    
    std::string m_name;
    std::string m_svgPath;
    std::string m_backgroundTextureName;
};

#endif // SPELLS_LEVEL_H
