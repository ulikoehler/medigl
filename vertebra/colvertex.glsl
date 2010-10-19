const vec3 coolest = vec3(0.784313725, 1.0, 1.0);
const vec3 hottest = vec3(1.0, 0.0, 0.0);
attribute float density;
void main()
{
	vec3 color = mix(coolest, hottest, density);
	gl_FrontColor = vec4(color, 1.0);
	gl_BackColor = gl_FrontColor;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
