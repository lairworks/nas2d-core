#version 150

out vec4 fragColor;

in vec2 out_pos;

uniform sampler2D tex0;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    fragColor = texture(tex0, out_pos);
}