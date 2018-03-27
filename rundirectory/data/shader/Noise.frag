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

#version 120

const float PI = 3.1415926535897932384626433832795;

// read more about faking random in GLSL at: https://thebookofshaders.com/10/
// 1337.0 * PI gives a smoother noise with less jumps in my tests than the usual 43758.5453123
float rand(vec2 coord){
    return fract(sin(dot(coord.xy ,vec2(12.9898,78.233))) * 1337.0 * PI);
}

uniform sampler2D    backgroundTexture;
uniform float        time;

void main( void )
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 textureColor = texture2D(backgroundTexture, uv);

    // generate noise
    uv = floor(uv * 100.0) / 100.0; // make the noise chunks bigger
    uv += vec2(cos(time), sin(time)) * 0.000005; // change coords by a tiny amount over time
    float noise = rand(uv) * 1.5; // brighten up
    vec4 noiseColor = vec4(noise, noise, noise, noise);

    // we want only the white parts the noise to show up,
    // so we blend it with the texture using the noise level as alpha
    noiseColor = mix(textureColor, noiseColor, noiseColor.a);

    // mix the noise onto opaque parts of the texture
    noiseColor = mix(textureColor, noiseColor, textureColor.a);

    // mix the noise with the texture
    gl_FragColor = gl_Color * mix(textureColor, noiseColor, 0.2);
}
