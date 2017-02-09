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

#version 120

uniform vec2 leftTop     = vec2(0.0, 0.1);
uniform vec2 rightBottom = vec2(0.9, 1.0);

void main(void)
{
    vec2 uv = gl_TexCoord[0].xy;
    const vec2 center = vec2(0.5, 0.5);

    vec4 vertexColor = gl_Color;
    vec4 opaqueColor = vec4(gl_Color.rgb, 0.0);

    // Bidirectional double smoothstep
    // see: http://www.fundza.com/rman_shaders/smoothstep/
    float blend = smoothstep(leftTop.x, leftTop.y, uv.x) * (1 - smoothstep(rightBottom.x, rightBottom.y, uv.x)) *
                  smoothstep(leftTop.x, leftTop.y, uv.y) * (1 - smoothstep(rightBottom.x, rightBottom.y, uv.y));

    gl_FragColor = mix(opaqueColor, vertexColor, blend);
}
