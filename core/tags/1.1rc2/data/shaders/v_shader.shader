#version 150
uniform mat4 mvp;

in vec3 position;
in vec2 texCoord;

out vec2 out_pos;

void main()
{
	out_pos = texCoord;
    gl_Position = mvp * vec4(position, 1);
}