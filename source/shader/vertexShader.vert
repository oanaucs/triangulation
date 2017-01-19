#version 330

layout(location = 0) in vec2 position;

out vec4 outColor;

uniform mat4 Projection;
uniform mat4 Model;
uniform mat4 View;
uniform vec4 Color;

void main()
{
	gl_Position = Projection  * View * Model * vec4(position, 0.0, 1.0);
	/*if (Color.y > 0.0f)
	{
		outColor = vec4(1.0, 1.0, 0.0, 1.0);
	}
	else
	{
		//outColor = vec4(0.0, 0.0, 1.0, 1.0);
		outColor = Color;
	}*/
	outColor = Color;
}
