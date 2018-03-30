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

#ifndef SPELLS_PARTICLE_SETUP_HPP
#define SPELLS_PARTICLE_SETUP_HPP

#include <map>
#include <vector>

#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Resources/ResourceHolder.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/SoundBuffer.hpp>


using EmitterMap  = std::map<std::string, std::function<void(thor::EmissionInterface&, sf::Time)>>;
using AffectorMap = std::map<std::string, std::function<void(thor::Particle&, sf::Time)>>;

EmitterMap  setupEmitters(std::vector<sf::Vector2f>& winPointVector);
AffectorMap setupAffectors();
thor::ResourceHolder<sf::Texture, std::string> loadTextures();
thor::ResourceHolder<sf::SoundBuffer, std::string> loadSounds();
thor::ResourceHolder<sf::Shader, std::string> loadShaders();

#endif // SPELLS_PARTICLE_SETUP_HPP
