#version 330

// TODO: get values from fragment shader
in vec3 frag_position;
in vec3 frag_color;
in vec3 frag_normala;
in vec2 frag_textura;


layout(location = 0) out vec4 out_color;
uniform float Time;
uniform vec3 Initial_Color;

void main()
{
	// TODO: write pixel out color

    //frag_color *= Time;

    out_color = vec4(vec3(0, max((1 - Time) * 0.8, 0.2), 0), 1);
	//out_color = vec4(frag_normala, 1);
}