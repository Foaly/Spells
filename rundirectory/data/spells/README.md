The `.spell` file format
======================

The `.spell` file format is a very simple settings format.
It is based on my [SettingsParser](https://github.com/Foaly/SettingsParser) project.
Each `.spell` file in this folder describes the appearance of a spell.
The program offers a set of variables that can be set for each spell to make them unique.
This allows you to create your own new spells easily.
Each line of the a `.spell` file contains one assignment.
So the syntax is a basic `key = value` pair.
Lines starting with a `#` are ignored and treated as comments.


Here is a list of all the variables you can assign for each spell.

Note: If the same variable is assigned more than once it will be overwritten with the later one.



Required variables
==================

These variables are required to be set for each spell.

`name`
------

Example: `name = Alohomora`

The name of the spell.


`path`
------

Example: `path = Alohomora.svg`

Name of the SVG file that contains the SVG paths describing the spell.
This is the white dotted shape you have to redraw.
The SVG files have to be located in the `data/svg/` folder.



Textures
========

`backgroundTextureName`
-----------------------

Example: `backgroundTextureName = door.png`

Name of the file that contains the background texture.
The background texture files have to be located in the `data/textures/` folder.


`emitterTexture`
----------------

Example: `emitterTexture = key.png`

Name of the file that contains the texture that is emitted on a successful spell.
The emitter texture files have to be located in the `data/textures/` folder.


`spellColor`
------------

Example: `spellColor = 0, 55, 224, 200`

The color of the spell.
Colors are written as four numbers seperated by commas representing a RGBA value.
Each number has to be in the range [0, 255].
Bigger or smaller values are clamped.
Exactly four numbers are required in order to parse a color correctly, otherwise the default color is used.
The order is `red, green, blue, alpha`.
The default color is a slightly transparent red (`165, 0, 0, 200`).



Sounds
======

`sound`
-------

Example: `sound = birds_taking_off.wav`

Filename of the sound that is played when the spell is successfully finished.
The sound files have to be located in the `data/sounds/` folder.



Emitters
========

`emitter`
---------

Example: `emitter = circularEmitter`

Chooses the emitter that is used for the successful spell effect.
Currently it is only possible to choose one emitter.
You can currently choose:

|          Name          | Effect                                                                                                                     |
|:----------------------:|:---------------------------------------------------------------------------------------------------------------------------|
|    circularEmitter     | Emits randomly rotating particles in all directions.                                                                       |
| scaledCircularEmitter  | Emits randomly rotating particles in all directions scaled between 0.6 and 0.7.                                            |
|       upEmitter        | Emits particles going upwards with a deflection of 60°. The particle texture is flipped into the moving direction.         |
| scaledRotatedUpEmitter | Emits particles going upwards with a deflection of 40°. The particles are randomly rotated and sclaed between 0.5 and 0.8. |


`affectors`
-----------

Example: `affectors = toTransparent, slowScaleUp`

Affectors affect the particles in the successful spell effect, after they have been created.
You can choose as many of the following affectors as you like:

|     Name      | Effect                                        |
|:-------------:|:----------------------------------------------|
|   downwards   | Lets the particle fall downward.              |
| toTransparent | Changes the particles opacity to transparent. |
|  slowScaleUp  | Slowly scales the particle up.                |
|  fastScaleUp  | Scales the particle up fast.                  |


`particleDownsampleFactor`
--------------------------

Example: `particleDownsampleFactor = 2`

By default for every spell point one particle is created during the successful spell effect.
This variable controls how many point are discarded.
For example a value of 2 only creates a particle for every second spell point.
The default value is 1.
The dowsample factor is clamped to the range [1, 10].
