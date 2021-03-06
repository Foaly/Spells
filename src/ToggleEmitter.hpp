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

// Created by Maximilian on 25.03.2018

#ifndef SPELLS_TOGGLEEMITTER_HPP
#define SPELLS_TOGGLEEMITTER_HPP

#include <Thor/Particles/Emitters.hpp>

class ToggleEmitter : public thor::UniversalEmitter {
public:
    ToggleEmitter(bool isOn = true) : m_isOn(isOn) {}

    void operator() (thor::EmissionInterface& system, sf::Time dt)
    {
        if (m_isOn)
            thor::UniversalEmitter::operator ()(system, dt);
    }

    void show(bool isOn)
    {
        m_isOn = isOn;
    }

private:
    bool m_isOn;
};

#endif // SPELLS_TOGGLEEMITTER_HPP
