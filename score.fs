#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D texture1;
uniform int show;
void main()
{
	
	vec4 texColor = texture(texture1, TexCoord);
	if(texColor.a < 0.1)
	discard;
	if (show==0){
		texColor.r = 0.0;
		texColor.g = 0.0;
		texColor.b = 0.0;
	}
	FragColor = texColor;
}