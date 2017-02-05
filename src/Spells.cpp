//
// Created by Maximilian on 19.06.2015.
//
#include <iostream>
#include <sstream>

#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Thor/Math.hpp>
#include <Thor/Resources/SfmlLoaders.hpp>
#include <Thor/Particles/Affectors.hpp>
#include <Thor/Animations/ColorAnimation.hpp>

#include "Spells.hpp"
#include "Util.hpp"
#include "VectorEmitter.hpp"
#include "PathResolver.hpp"

namespace
{
    template <typename T>
    std::string toString ( T Number )
    {
        std::stringstream ss;
        ss << Number;
        return ss.str();
    }
}

Spells::Spells() : m_isUserDrawing(false),
                   m_isComputing(false),
                   m_window(sf::VideoMode(1280, 800), "Spells", sf::Style::Fullscreen),
                   m_windowCenter(m_window.getSize().x / 2, m_window.getSize().y / 2),
                   m_userPointRadius(20.f),
                   m_spellGenerator(m_windowCenter)
{
    // limit frame time
    m_window.setFramerateLimit(60);

    // load textures
    m_textures.acquire("circle", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/Circle.png")), thor::Resources::Reuse);
    m_textures.acquire("rect", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/Rect.png")), thor::Resources::Reuse);
    m_textures.acquire("star", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/Star.png")), thor::Resources::Reuse);
    m_textures.acquire("key", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/old key small.png")), thor::Resources::Reuse);
    m_textures.acquire("arches", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/arches.png")), thor::Resources::Reuse);
    m_textures.acquire("door", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/door.png")), thor::Resources::Reuse);
    m_textures.acquire("green house", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/green house.png")), thor::Resources::Reuse);
    m_textures.acquire("wand", thor::Resources::fromFile<sf::Texture>(resolvePath("data/textures/wand.png")), thor::Resources::Reuse);
    
    std::string bgTextureName = "door";
    
    // set up background
    m_textures[bgTextureName].setSmooth(true);
    m_backgroundSprite.setTexture(m_textures[bgTextureName]);
    sf::Vector2f scale(static_cast<float>(m_window.getSize().x) / m_textures[bgTextureName].getSize().x, static_cast<float>(m_window.getSize().y) / m_textures[bgTextureName].getSize().y);
    m_backgroundSprite.setScale(scale);
    std::cout << "Background image scale: " << scale.x << "x" << scale.y << std::endl;
    
    m_overlayRect.setSize(sf::Vector2f(m_window.getSize().x - 200, m_window.getSize().y - 50));
    m_overlayRect.setPosition(100, 25);
    m_overlayRect.setFillColor(sf::Color(0, 0, 0, 100));
    m_overlayRect.setTexture(&m_textures["rect"]);
    
    m_window.setMouseCursorVisible(false);
    m_textures["wand"].setSmooth(true);
    m_wand.setTexture(m_textures["wand"]);
    m_wand.setScale(-1.f, 1.f); // uncomment for left hand
    m_wand.setOrigin(94.f, 3.f);

    // set up the particle systems
    m_winParticleSystem.setTexture(m_textures["key"]);

    m_failParticleSystem.setTexture(m_textures["circle"]);
    
    m_fallingPointParticleSystem.setTexture(m_textures["circle"]);
    m_fallingPointParticleSystem.addEmitter(thor::refEmitter(m_fallingPointEmitter));
    
    m_textures["star"].setRepeated(false);
    m_wandParticles.setTexture(m_textures["star"]);
    m_wandParticles.addTextureRect(sf::IntRect(0, 0, 12, 12));
    m_wandParticles.addTextureRect(sf::IntRect(12, 0, 24, 12));
    m_wandEmitter.setEmissionRate(30.f);
    m_wandEmitter.setParticleLifetime( thor::Distributions::uniform(sf::seconds(0.8f), sf::seconds(1.0f)));
    m_wandEmitter.setParticlePosition( thor::Distributions::circle(sf::Vector2f(300, 300), 10) );
    m_wandEmitter.setParticleVelocity( util::Distributions::disk(60.f, 100.f) );
    m_wandEmitter.setParticleRotation( thor::Distributions::uniform(0.f, 360.f) );
    m_wandEmitter.setParticleRotationSpeed( thor::Distributions::uniform(40.f, 60.f));
    m_wandEmitter.setParticleTextureIndex( thor::Distributions::uniform(0, 1));
    m_wandParticles.addEmitter(thor::refEmitter(m_wandEmitter));
    
    thor::ForceAffector forceAffector(sf::Vector2f(0, 150));
    m_wandParticles.addAffector(forceAffector);
    
    thor::ColorGradient goldGradient;
    goldGradient[0.0f] = sf::Color(255, 200, 50, 200); // slightly transparent gold
    goldGradient[0.8f] = sf::Color(255, 200, 50, 200);
    goldGradient[1.0f] = sf::Color(255, 200, 50,   0); // transparent gold
    thor::ColorAnimation goldToTransparent(goldGradient);
    m_wandParticles.addAffector(thor::AnimationAffector(goldToTransparent));
    
    // font loading and text setup
    if(!m_font.loadFromFile(resolvePath("data/fonts/BilboSwashCaps-Regular.otf")))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    m_percentageText.setFont(m_font);
    m_percentageText.setString("0%");
    m_percentageText.setCharacterSize(50);
    m_percentageText.setPosition(30, 10);
    m_percentageText.setStyle(sf::Text::Style::Bold);
    // TODO horizontal character spacing

    if(sf::Shader::isAvailable())
    {
        if(!m_radialGradientShader.loadFromFile(resolvePath("data/shader/RadialGradient.frag"), sf::Shader::Fragment))
        {
            std::cerr << "Failed to load radial gradient shader!" << std::endl;
        }
        if(!m_rectangleGradientShader.loadFromFile(resolvePath("data/shader/RectangleGradient.frag"), sf::Shader::Fragment))
        {
            std::cerr << "Failed to load radial gradient shader!" << std::endl;
        }
        m_rectangleGradientShader.setUniform("leftTop", sf::Vector2f(0.f, 0.05f));
        m_rectangleGradientShader.setUniform("rightBottom", sf::Vector2f(0.95f, 1.0));
    }

    if(!m_failSoundBuffer.loadFromFile(resolvePath("data/sounds/fail.wav")))
    {
        std::cerr << "Failed to load the fail sound!" << std::endl;
    }

    m_failSound.setBuffer(m_failSoundBuffer);

    //m_spellPoints = m_spellGenerator.generateSpirale();
    //m_spellPoints = m_spellGenerator.generateWave();

    //BezierCurve curve(sf::Vector2f(300, 400), sf::Vector2f(500, 100), sf::Vector2f(700, 700), sf::Vector2f(900, 400));
    //m_spellPoints = curve.generateEvenlySpacedPoints(20.f); // distance of 20px between points
    
    m_spellPoints = loadPathsFromFile(resolvePath("data/svg/Alohomora.svg"));


    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    std::cout << "Thor version: " << THOR_VERSION_MAJOR << "." << THOR_VERSION_MINOR << std::endl;
    std::cout << std::endl;
}


int Spells::run()
{
    // run the program as long as the m_window is open
    while (m_window.isOpen())
    {
        // handle events
        handleEvents();

        // update everything
        update();

        // draw everything
        draw();
    }

    return 0;
}



void Spells::handleEvents()
{
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // "close requested" event: close the window
        if (event.type == sf::Event::Closed)
            m_window.close();

        else if(event.type == sf::Event::KeyReleased)
        {
            switch(event.key.code)
            {
                // close if escape key was pressed
                case sf::Keyboard::Escape:
                    m_window.close();
                    break;
                // delete all the user points
                case sf::Keyboard::C:
                    m_userPoints.clear();
                    m_percentageText.setString("0%");
                    break;
                default:
                    break;
            }
        }

        else if(event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if(!m_isComputing)
                {
                    // the user starts drawing
                    m_isUserDrawing = true;
                    const sf::Vector2i pixelPosition(event.mouseButton.x, event.mouseButton.y);
                    m_lastPosition = m_window.mapPixelToCoords(pixelPosition);
                    m_userPoints.push_back(m_lastPosition);
                }
            }
        }
        else if(event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if(!m_isComputing)
                {
                    // the user has stopped drawing
                    // set up everything for the calculation of the hit percentage
                    m_isUserDrawing = false;
                    m_isComputing = true;
                    m_spellPointsCopy = m_spellPoints; // make a copy of the spell points
                    m_userPointIter = m_userPoints.begin();
                    m_numberOfPointsHit = 0;
                    m_computingClock.restart();
                    m_wandParticles.clearEmitters();
                }
            }
        }
    }
}


void Spells::update()
{
    // get frame time
    const sf::Time frameTime = m_frameClock.restart();
    
    // get mouse position
    const sf::Vector2f mousePosition(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
    
    // update wand
    m_wand.setPosition(mousePosition);
    m_wandEmitter.setParticlePosition(mousePosition );
    
    if(m_isUserDrawing)
    {
        const sf::Vector2f delta(mousePosition - m_lastPosition);
        const float movementLength = thor::length(delta);

        // add a point for every 20 pixel the mouse moves between two frames
        if(movementLength > m_userPointRadius)
        {
            const sf::Vector2f direction(thor::unitVector(delta));
            const int steps = static_cast<int>(std::ceil(movementLength)) / static_cast<int>(m_userPointRadius);
            for(int i = 0; i < steps; i++)
            {
                m_lastPosition += direction * m_userPointRadius;
                m_userPoints.push_back(m_lastPosition);
            }
        }
    }
    else if(m_isComputing)
    {
        if(m_computingClock.getElapsedTime() >= sf::milliseconds(30)) // this clock determines the speed at which the wrong points fall
        {
            bool didUserPointHit = false;
            const sf::Vector2f userPointPosition(*m_userPointIter);

            // check if the user point we are currently looking at is close to any spell point
            for (auto iter = m_spellPointsCopy.begin(); iter != m_spellPointsCopy.end();)
            {
                // calculate the distance between the current user point the current spell point
                const sf::Vector2f spellPointPosition(*iter);
                const sf::Vector2f delta(spellPointPosition - userPointPosition);
                const float distance = thor::length(delta);

                if (distance < 22.f)
                {
                    m_numberOfPointsHit++;
                    didUserPointHit = true;
                    m_spellPointsCopy.erase(iter);
                    break;
                }
                else
                {
                    iter++;
                }
            }

            if (didUserPointHit)
            {
                // if the user point is close to a spell point simply keep going
                m_userPointIter++;
            }
            else
            {
                // if the user point is not near any spell point remove it and make a falling point animation
                const sf::Vector2f position = *m_userPointIter;
                m_fallingPointEmitter.emitParticle(position);
                m_userPointIter = m_userPoints.erase(m_userPointIter);
            }

            // calculate the percentage of points hit
            const float percent = std::round(static_cast<float>(m_numberOfPointsHit) / m_spellPoints.size() * 100.f);

            // TODO: this should use std::to_string. This is a compiler error an update should help
            m_percentageText.setString(toString(percent) + "%");

            // if we have checked all user points play an animation if enough percent are covert
            if (m_userPointIter == m_userPoints.end())
            {
                std::cout << "Hits: " << m_numberOfPointsHit << " Percent: " << percent << std::endl;

                // if more than 70% are covered play an animation
                if(percent > 70.f)
                {
                    VectorEmitter emitter(m_spellPoints);
                    emitter.setEmissionRate(20);
                    emitter.setParticleLifetime( thor::Distributions::uniform(sf::seconds(1.2f), sf::seconds(1.6f)) );
                    emitter.setParticleVelocity( util::Distributions::disk(100.f, 200.f) );   // Emit particles with a velocity between 100.f and 200.f in a random direction
                    emitter.setParticleRotation( thor::Distributions::uniform(0.f, 360.f) );      // Rotate randomly
                    emitter.setParticleRotationSpeed( thor::Distributions::uniform(10.f, 50.f));  // random rotation speed
                    m_winParticleSystem.addEmitter(emitter, sf::seconds(2.f));
                }
                else
                {
                    // move the failed points
                    m_failedPoints = std::move(m_userPoints);

                    // generate a particle for each failed point
                    VectorEmitter emitter(m_failedPoints);
                    emitter.setEmissionRate(0);
                    emitter.setParticleLifetime( util::Distributions::constant(sf::seconds(1.8f)) );
                    m_failParticleSystem.addEmitter(emitter, sf::seconds(2.f));
                    
                    // scale the particles up
                    thor::ScaleAffector scaleUp(sf::Vector2f(1.1, 1.1));
                    m_failParticleSystem.addAffector(scaleUp, sf::seconds(2.f));
                    
                    // fade the points from red to black to transparent
                    thor::ColorGradient toBlackGradient;
                    toBlackGradient[0.0f] = sf::Color(165,  0,  0, 200); // slightly transparent red
                    toBlackGradient[0.9f] = sf::Color( 20, 20, 20,  50); // slightly opaque grey
                    toBlackGradient[1.0f] = sf::Color(  0,  0,  0,   0); // transparent
                    thor::ColorAnimation toBlack(toBlackGradient);
                    m_failParticleSystem.addAffector(thor::AnimationAffector(toBlack), sf::seconds(2.f));
                    
                    // play a fail sound and reset the percent text
                    m_failSound.play();
                    m_percentageText.setString("0%");
                }

                m_isComputing = false;
                m_wandParticles.addEmitter(thor::refEmitter(m_wandEmitter));
            }

            m_computingClock.restart();
        }
    }

    // update particle system
    m_winParticleSystem.update(frameTime);
    m_failParticleSystem.update(frameTime);
    m_fallingPointParticleSystem.update(frameTime);
    m_wandParticles.update(frameTime);
}


void Spells::draw()
{
    // clear the m_window with black color
    m_window.clear(sf::Color::Black);
    
    // draw background
    m_window.draw(m_backgroundSprite);
    
    m_window.draw(m_overlayRect, &m_rectangleGradientShader);

    // draw the win particle system
    m_window.draw(m_winParticleSystem);

    // draw the spell
    m_radialGradientShader.setUniform("radiuses", sf::Vector2f(0.5f, 0.4f));
    sf::Sprite circle(m_textures["circle"]);
    circle.setOrigin(25, 25);
    circle.setColor(sf::Color(255, 255, 255, 100));
    for(auto position: m_spellPoints)
    {
        circle.setPosition(position);
        m_window.draw(circle, &m_radialGradientShader);
    }

    // draw the falling user points
    m_radialGradientShader.setUniform("radiuses", sf::Vector2f(0.5f, 0.3f));
    m_window.draw(m_fallingPointParticleSystem, &m_radialGradientShader);
    
    // draw the fail particle system
    m_window.draw(m_failParticleSystem, &m_radialGradientShader);

    // draw the user spell
    circle.setScale(0.8f, 0.8f);
    circle.setColor(sf::Color(165, 0, 0, 200));
    for(auto position: m_userPoints)
    {
        circle.setPosition(position);
        m_window.draw(circle, &m_radialGradientShader);
    }

    // draw percentage text
    m_window.draw(m_percentageText);
    
    // draw wand
    m_window.draw(m_wandParticles);
    m_window.draw(m_wand);

    // end the current frame
    m_window.display();
}
