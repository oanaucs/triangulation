#version 330

in vec4 outColor;
out vec4 out_Color;

void main()
{
	out_Color = vec4(outColor.x, outColor.y, outColor.z, 1.0f);
	/*if (outColor.y < 0.5f)
	{
		out_Color = vec4(outColor.x, outColor.y, outColor.z, 1.0);	
	}
    else
	{
		out_Color = vec4(outColor.x, outColor.y, outColor.z, 1.0f);	
	}*/
}

