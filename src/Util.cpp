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

// File created by Maximilian on 26.07.2015.


#include "Util.hpp"
#include "BezierCurve.hpp"

#include <Thor/Math/Random.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Thor/Vectors/PolarVector2.hpp>

#include <iostream>
#include <iterator>

#include <stdio.h>
#include <string.h>
#include <math.h>
#define NANOSVG_IMPLEMENTATION  // Expands implementation
#include "nanosvg/nanosvg.h"

namespace util
{
    namespace Distributions
    {

        thor::Distribution<sf::Vector2f> disk(float minRadius, float maxRadius, sf::Vector2f center)
        {
            assert(minRadius >= 0.f);
            assert(maxRadius > minRadius);

            return [=] () -> sf::Vector2f
            {
                sf::Vector2f radiusVector = thor::PolarVector2f(thor::random(minRadius, maxRadius), thor::random(0.f, 360.f));
                return center + radiusVector;
            };
        }
        
        
        thor::Distribution<sf::Vector2f> deflect( thor::Distribution<sf::Vector2f> direction, float maxAngle)
        {
            return [=] () -> sf::Vector2f
            {
                return thor::rotatedVector(direction(), thor::randomDev(0.f, maxAngle));
            };
        }
        
        
        thor::Distribution<sf::Vector2f> uniform (sf::Vector2f min, sf::Vector2f max)
        {
            return [=] () -> sf::Vector2f
            {
                return sf::Vector2f(thor::random(min.x, max.x), thor::random(min.y, max.y));
            };
        }
        

        thor::Distribution<sf::Time> constant(sf::Time time)
        {
            return [=] () -> sf::Time
            {
                return time;
            };
        }


    } // namespace Distributions
} // namespace util


std::vector<sf::Vector2f> loadPathsFromFile(std::string filename)
{
    // Load file
    NSVGimage* image;
    image = nsvgParseFromFile(filename.c_str(), "px", 96.f);
    std::vector<sf::Vector2f> result;
    if (image == NULL)
    {
        std::cout << "Could not open SVG image: " << filename << std::endl;
        return result;
    }
    std::cout << "Size: " << image->width << "x" << image->height << std::endl;

    for (NSVGshape *shape = image->shapes; shape != NULL; shape = shape->next)
    {
        for (NSVGpath *path = shape->paths; path != NULL; path = path->next)
        {
            for (int i = 0; i < path->npts-1; i += 3)
            {
                float* p = &path->pts[i*2];

                BezierCurve curve(sf::Vector2f(p[0],p[1]), sf::Vector2f(p[2],p[3]), sf::Vector2f(p[4],p[5]), sf::Vector2f(p[6],p[7]));
                auto points = curve.generateEvenlySpacedPoints(20.f);
                result.insert(std::end(result), std::begin(points), std::end(points));
            }
        }
    }

    // free memory
    nsvgDelete(image);

    return result;
}
