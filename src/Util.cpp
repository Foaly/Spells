//
// Created by Maximilian on 26.07.2015.
//

#include "Util.hpp"
#include "BezierCurve.hpp"

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


        thor::Distribution<sf::Time> constant(sf::Time time)
        {
            return time;
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
