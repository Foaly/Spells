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


#ifndef SPELLS_UTIL_H
#define SPELLS_UTIL_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <Thor/Math/Distributions.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Thor/Vectors/PolarVector2.hpp>

#include <cassert>
#include <vector>
#include <string>

namespace util
{
    namespace Distributions
    {
        ///
        /// \brief A uniform random distribution in a certain range of a circle. Comparable to a disk.
        ///
        thor::Distribution<sf::Vector2f> disk(float minRadius, float maxRadius, sf::Vector2f center = sf::Vector2f(0.f, 0.f));

        ///
        /// \brief A constant "distribution". Always returns the same value
        ///
        thor::Distribution<sf::Time> constant(sf::Time time);
    }
}

std::vector<sf::Vector2f> loadPathsFromFile(std::string filename);


#endif //SPELLS_UTIL_H
