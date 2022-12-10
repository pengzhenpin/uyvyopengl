#version 330 core
out vec4 color;
in vec2 coord;
uniform sampler2D tex;

const mat3 yuvMat = mat3(1.16414, -0.0011, 1.7923,
                        1.16390, -0.2131, -0.5342,
                        1.16660, 2.1131, -0.0001);
const vec3 yuvVec = vec3(-0.9726, 0.3018, -1.1343);

void main(void) {
    ivec2 size = textureSize(tex, 0);
    size.x *= 2;
    ivec2 pos = ivec2(coord.x * size.x, coord.y * size.y);
    vec3 yuv;

    if (pos.x % 2 == 0) {
        yuv = texelFetch(tex, ivec2(pos.x / 2, pos.y), 0).grb;
    } else {
        yuv = texelFetch(tex, ivec2(pos.x / 2, pos.y), 0).arb;
    }

    vec4 tmp = vec4 (yuv.r, yuv.g, yuv.b, 1);
    vec3 rgb = vec3( dot( yuvMat[0], tmp.rgb ),
                             dot( yuvMat[1], tmp.rgb ),
                             dot( yuvMat[2], tmp.rgb )) + yuvVec;
    color = vec4(rgb, 1);
}
