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

#include "LoadHelpers.hpp"

#include "Util.hpp"
#include "VectorEmitter.hpp"
#include "PathResolver.hpp"
#include "Affectors.hpp"

#include <Thor/Particles/Affectors.hpp>
#include <Thor/Animations/ColorAnimation.hpp>
#include <Thor/Resources/SfmlLoaders.hpp>

#include <SFML/Audio/SoundBuffer.hpp>

#include <iostream>


EmitterMap setupEmitters(std::vector<sf::Vector2f>& winPointVector)
{
    EmitterMap emitters;

    VectorEmitter circularEmitter(winPointVector);
    circularEmitter.setParticleLifetime( thor::Distributions::uniform(sf::seconds(1.8f), sf::seconds(2.2f)) );
    circularEmitter.setRandomOrientation(true);
    emitters["nonmovingEmitter"] = circularEmitter;

    circularEmitter.setParticleLifetime( thor::Distributions::uniform(sf::seconds(1.2f), sf::seconds(1.6f)) );
    circularEmitter.setRandomOrientation(false);
    circularEmitter.setEmissionRate(20);
    circularEmitter.setParticleVelocity( util::Distributions::disk(100.f, 200.f) );       // emit particles with a velocity between 100.f and 200.f in a random direction
    circularEmitter.setParticleRotation( thor::Distributions::uniform(0.f, 360.f) );      // rotate randomly
    circularEmitter.setParticleRotationSpeed( thor::Distributions::uniform(10.f, 50.f));  // random rotation speed
    emitters["circularEmitter"] = circularEmitter;

    circularEmitter.setParticleScale( util::Distributions::uniformScale(0.6f, 0.7f) ); // start with a lower scale
    emitters["scaledCircularEmitter"] = circularEmitter;

    VectorEmitter upEmitter(winPointVector);
    upEmitter.setFlipTowardsDirection(true);
    upEmitter.setEmissionRate(10);
    upEmitter.setParticleLifetime( thor::Distributions::uniform(sf::seconds(1.0f), sf::seconds(1.2f)) );
    auto verticalSpeed = util::Distributions::uniform(sf::Vector2f(0.f, -250.f), sf::Vector2f(0.f, -200.f));
    upEmitter.setParticleVelocity( util::Distributions::deflect(verticalSpeed, 60.f) );
    emitters["upEmitter"] = upEmitter;

    upEmitter.setFlipTowardsDirection(false); // turn flipping off, otherwise scaling looks weird
    upEmitter.setParticleRotation( thor::Distributions::uniform(0.f, 360.f) );   // rotate randomly
    upEmitter.setParticleScale( util::Distributions::uniformScale(0.5f, 0.8f) ); // start with a lower scale
    upEmitter.setParticleVelocity( util::Distributions::deflect(verticalSpeed, 40.f) );
    emitters["scaledRotatedUpEmitter"] = upEmitter;

    return emitters;
}


AffectorMap setupAffectors()
{
    AffectorMap affectors;

    // gravity
    thor::ForceAffector downwardsAffector(sf::Vector2f(0, 150));
    affectors["downwards"] = downwardsAffector;

    // golden color that fades to transparent
    thor::ColorGradient goldGradient;
    goldGradient[0.0f] = sf::Color(255, 200, 50, 200); // slightly transparent gold
    goldGradient[0.8f] = sf::Color(255, 200, 50, 200); // slightly transparent gold
    goldGradient[1.0f] = sf::Color(255, 200, 50,   0); // transparent gold
    thor::ColorAnimation goldToTransparent(goldGradient);
    affectors["goldToTransparent"] = thor::AnimationAffector(goldToTransparent);

    // gentle fade to transparent
    thor::ColorGradient toTransarentGradient;
    toTransarentGradient[0.0f] = sf::Color( 255, 255, 255, 255); // opaque
    toTransarentGradient[0.9f] = sf::Color( 255, 255, 255, 150); // slightly transparent
    toTransarentGradient[1.0f] = sf::Color( 255, 255, 255,   0); // transparent
    thor::ColorAnimation toTansparent(toTransarentGradient);
    affectors["toTransparent"] = thor::AnimationAffector(toTansparent);

    // quick transition to transparent at the end
    toTransarentGradient[0.9f] = sf::Color( 255, 255, 255, 255); // still opaque
    thor::ColorAnimation toTansparentEnd(toTransarentGradient);
    affectors["toTransparentEnd"] = thor::AnimationAffector(toTansparentEnd);

    // scale the particles up slowly
    ScaleAffector slowScaleUp(sf::Vector2f(0.6, 0.6));
    affectors["slowScaleUp"] = slowScaleUp;

    // scale the particles up faster
    ScaleAffector fastScaleUp(sf::Vector2f(1.1, 1.1));
    affectors["fastScaleUp"] = fastScaleUp;

    // scale the particles down slowly
    ScaleAffector slowScaleDown(sf::Vector2f(-0.6, -0.6));
    affectors["slowScaleDown"] = slowScaleDown;

    // move in direction ov the center
    DirectedAffector directedAffector(sf::Vector2f(724, 416));
    affectors["directedCenter"] = directedAffector;

    // quickly scales up along the x axis
    // and scales back down shortly before the end
    ScaleXUpDownAffector scaleXUpDownAffector;
    affectors["scaleXUpDownAffector"] = scaleXUpDownAffector;

    return affectors;
}


thor::ResourceHolder<sf::Texture, std::string> loadTextures()
{
    thor::ResourceHolder<sf::Texture, std::string> textures;

    // load textures
    textures.acquire("circle.png",       thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/circle.png")),       thor::Resources::Reuse);
    textures.acquire("rect.png",         thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/rect.png")),         thor::Resources::Reuse);
    textures.acquire("star.png",         thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/star.png")),         thor::Resources::Reuse);
    textures.acquire("key.png",          thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/key.png")),          thor::Resources::Reuse);
    textures.acquire("arches.png",       thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/arches.png")),       thor::Resources::Reuse);
    textures.acquire("door.png",         thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/door.png")),         thor::Resources::Reuse);
    textures.acquire("green_house.png",  thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/green_house.png")),  thor::Resources::Reuse);
    textures.acquire("wand.png",         thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/wand.png")),         thor::Resources::Reuse);
    textures.acquire("yellow_bird.png",  thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/yellow_bird.png")),  thor::Resources::Reuse);
    textures.acquire("orange_orb.png",   thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/orange_orb.png")),   thor::Resources::Reuse);
    textures.acquire("parchment.png",    thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/parchment.png")),    thor::Resources::Reuse);
    textures.acquire("stairs_top.png",   thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/stairs_top.png")),   thor::Resources::Reuse);
    textures.acquire("fire.png",         thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/fire.png")),         thor::Resources::Reuse);
    textures.acquire("feather.png",      thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/feather.png")),      thor::Resources::Reuse);
    textures.acquire("clock.png",        thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/clock.png")),        thor::Resources::Reuse);
    textures.acquire("jupiter.png",      thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/jupiter.png")),      thor::Resources::Reuse);
    textures.acquire("corridor.png",     thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/corridor.png")),     thor::Resources::Reuse);
    textures.acquire("redLightning.png", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/redLightning.png")), thor::Resources::Reuse);
    textures.acquire("water_orb.png",    thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/water_orb.png")),    thor::Resources::Reuse);
    textures.acquire("uranus.png",       thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/uranus.png")),       thor::Resources::Reuse);

    return textures;
}


thor::ResourceHolder<sf::SoundBuffer, std::string> loadSounds()
{
    thor::ResourceHolder<sf::SoundBuffer, std::string> sounds;

    // load sounds
    sounds.acquire("fail.wav",             thor::Resources::fromFile<sf::SoundBuffer>(resolvePath("data/sounds/fail.wav")),             thor::Resources::Reuse);
    sounds.acquire("birds_taking_off.wav", thor::Resources::fromFile<sf::SoundBuffer>(resolvePath("data/sounds/birds_taking_off.wav")), thor::Resources::Reuse);
    sounds.acquire("fire.wav",             thor::Resources::fromFile<sf::SoundBuffer>(resolvePath("data/sounds/fire.wav")),             thor::Resources::Reuse);
    sounds.acquire("opening_lock.wav",     thor::Resources::fromFile<sf::SoundBuffer>(resolvePath("data/sounds/opening_lock.wav")),     thor::Resources::Reuse);
    sounds.acquire("aguamenti.wav",        thor::Resources::fromFile<sf::SoundBuffer>(resolvePath("data/sounds/aguamenti.wav")),        thor::Resources::Reuse);

    // create an empty Soundbuffer as a fallback
    auto emptySoundbuffer = [=] () -> std::unique_ptr<sf::SoundBuffer>
    {
        return std::unique_ptr<sf::SoundBuffer>(new sf::SoundBuffer());
    };
    thor::ResourceLoader<sf::SoundBuffer> emptyLoader(emptySoundbuffer, "AudioNone");
    sounds.acquire("none", emptyLoader);

    return sounds;
}


thor::ResourceHolder<sf::Shader, std::string> loadShaders()
{
    thor::ResourceHolder<sf::Shader, std::string> shaders;

    auto fragmentShaderLoader = [] (const std::string &path) -> std::unique_ptr<sf::Shader>
    {
        if (!sf::Shader::isAvailable())
            return nullptr;

        auto shader = std::make_unique<sf::Shader>();
        if (!shader->loadFromFile(path, sf::Shader::Fragment))
        {
            std::cerr << "Failed to load shader: " << path << std::endl;
            return nullptr;
        }
        return shader;
    };

    auto loadShader = [=] (std::string filename) -> thor::ResourceLoader<sf::Shader>
    {
        return thor::ResourceLoader<sf::Shader>([=] () { return fragmentShaderLoader(filename); }, filename);
    };

    // TODO: each aquire should be in a seperate try catch
    try
    {
        shaders.acquire("RadialGradient.frag",    loadShader(resolvePath("data/shader/RadialGradient.frag")),    thor::Resources::Reuse);
        shaders.acquire("RectangleGradient.frag", loadShader(resolvePath("data/shader/RectangleGradient.frag")), thor::Resources::Reuse);
        shaders.acquire("Noise.frag",             loadShader(resolvePath("data/shader/Noise.frag")),             thor::Resources::Reuse);
    }
    catch (thor::ResourceLoadingException &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return shaders;
}
