#version 330 core
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;
layout (location = 3) in mat4 inTransform;

flat out vec3 passColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec2 lightBias;

uniform mat4 projection;
uniform mat4 view;


vec3 calculateLighting(){
	float brightness = max(dot(-lightDirection, inNormal), 0.0);
	return (lightColor * lightBias.x) + (brightness * lightColor * lightBias.y);
}

void main() {
 	passColor = inColor.rgb * calculateLighting();
    gl_Position = projection * view * inTransform * vec4(inVertex, 1.0);
}