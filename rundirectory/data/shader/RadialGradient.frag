#version 120

uniform float circleDiameter = 40.0;

void main(void)
{
    vec2 uv = gl_TexCoord[0].xy / circleDiameter;
    const vec2 center = vec2(0.5, 0.5);

    float distanceFromCenter = distance(uv, center);
    const vec2 radiuses = vec2(0.5, 0.3);

    vec4 vertexColor = gl_Color;
    vec4 opaqueColor = vec4(gl_Color.rgb, 0.0);

    float interpolator = 1.0 - smoothstep(radiuses.x, radiuses.y, distanceFromCenter);

    //gl_FragColor = vec4(uv.x, uv.y, 1.0, 1.0);
    gl_FragColor = mix(vertexColor, opaqueColor, interpolator);
}