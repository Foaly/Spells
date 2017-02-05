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
