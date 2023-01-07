#version 330

flat in vec3 passColor;

out vec4 outColor;


void main(void){
	outColor = vec4(passColor, 1.0);
}