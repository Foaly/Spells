#ifndef SPELLS_BEZIER_H
#define SPELLS_BEZIER_H

#include <SFML/System/Vector2.hpp>

#include <vector>

class BezierCurve
{
public:
    BezierCurve(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3);
    
    std::vector<sf::Vector2f> calculatePoints(std::size_t pointCount);
    
private:
    
    sf::Vector2f m_p0;
    sf::Vector2f m_p1;
    sf::Vector2f m_p2;
    sf::Vector2f m_p3;
};


#endif // SPELLS_BEZIER_H
