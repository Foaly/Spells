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

// File created by Maximilian on 26.07.2015.


#ifndef SPELLS_UTIL_H
#define SPELLS_UTIL_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <Thor/Math/Distributions.hpp>

#include <cassert>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <cmath>

namespace util
{
    namespace Distributions
    {
        ///
        /// \brief A uniform random distribution in a certain range of a circle. Comparable to a disk.
        ///
        thor::Distribution<sf::Vector2f> disk(float minRadius, float maxRadius, sf::Vector2f center = sf::Vector2f(0.f, 0.f));
        
        ///
        /// \brief A distribution towards a direction with a deviation of maxAngle
        ///
        thor::Distribution<sf::Vector2f> deflect( thor::Distribution<sf::Vector2f> direction, float maxAngle);
        
        ///
        /// \brief A uniform distribution between two vectors
        ///
        thor::Distribution<sf::Vector2f> uniform (sf::Vector2f min, sf::Vector2f max);
        
        ///
        /// \brief A uniform distribution where both vector elements are the same random value
        ///
        thor::Distribution<sf::Vector2f> uniformScale (float min, float max);
  
        ///
        /// \brief A constant "distribution". Always returns the same value
        ///
        template<class T>
        thor::Distribution<T> constant(T value)
        {
            return [=] () -> T
            {
                return value;
            };
        }
    }
}


std::vector<sf::Vector2f> loadPathsFromFile(std::string filename);


// Compute the normal of a segment
template<typename T>
sf::Vector2<T> computeNormal(const sf::Vector2<T>& p1, const sf::Vector2<T>& p2)
{
    sf::Vector2<T> normal(p1.y - p2.y, p2.x - p1.x);
    float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
    if (length != 0.f)
        normal /= length;
    return normal;
}


///
/// \brief Returns a container with every n element of the input container
///
template<class in_it, class out_it>
out_it downsample( in_it begin, in_it end, out_it result, std::size_t n)
{
    for (std::size_t i = std::distance(begin, end) / n; i--; std::advance(begin, n))
        *result++ = *begin;
    return result;
}


template <typename T>
T clamp(T value, T min, T max) {
    assert(min < max);
    return std::max(min, std::min(value, max));
}



// trim string inplace from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim string inplace from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim string inplace from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}


// taken from: https://stackoverflow.com/questions/1489830/efficient-way-to-determine-number-of-digits-in-an-integer
template <class T>
int numDigits(T number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}


#endif //SPELLS_UTIL_H
