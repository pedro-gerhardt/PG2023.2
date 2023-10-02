#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
uniform mat4 projection;
uniform mat4 model;

out vec4 vertexColor;

void main()
{
	gl_Position = projection * model * vec4(position, 1.0);
	vertexColor = vec4(color, 1.0);
}