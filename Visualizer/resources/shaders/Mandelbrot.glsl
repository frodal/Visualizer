#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position, 0.0, 1.0);
	v_TexCoord = texCoord;
};

#shader fragment
#version 400 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform dvec2 pos;
uniform double scale;
uniform int MaxIterations;
uniform float aspectRatio;

int SinglePrecision()
{
	float x0 = float(scale)*(2.0*aspectRatio*v_TexCoord.x-aspectRatio) + float(pos.x);
	float y0 = float(scale)*(2.0*v_TexCoord.y-1.0) + float(pos.y);
	
	float x = 0.0;
	float y = 0.0;
	
	float xtemp;
	
	int n = 0;

	while(x*x + y*y < 4.0 && n < MaxIterations)
	{
		xtemp = x*x - y*y + x0;
		y = 2.0*x*y + y0;
		x = xtemp;
		n++;
	}

	return n;
};

int doublePrecision()
{
	double x0 = scale*(2.0*aspectRatio*v_TexCoord.x-aspectRatio) + pos.x;
	double y0 = scale*(2.0*v_TexCoord.y-1.0) + pos.y;
	
	double x = 0.0;
	double y = 0.0;
	
	double xtemp;
	
	int n = 0;

	while(x*x + y*y < 4.0 && n < MaxIterations)
	{
		xtemp = x*x - y*y + x0;
		y = 2.0*x*y + y0;
		x = xtemp;
		n++;
	}

	return n;
};

void main()
{
	int n;

	if(scale<5e-5)
	{
		n = doublePrecision();
	}
	else
	{
		n = SinglePrecision();
	}

	float value = 300.0 * float(n) / float(MaxIterations);

	if(n==MaxIterations)
	{
		color = vec4(0.0, 0.0, 0.0, 1.0);
	}else
	{
		color = vec4(0.5 * sin(value) + 0.5, 0.5 * sin(value + 1.094) + 0.5, 0.5 * sin(value + 2.188) + 0.5, 1.0);
	}

};