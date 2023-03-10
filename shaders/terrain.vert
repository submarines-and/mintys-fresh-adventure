#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;

flat out vec3 passColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec2 lightBias;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

vec3 calculateLighting(){
	float brightness = max(dot(-lightDirection, inNormal), 0.0);
	return (lightColor * lightBias.x) + (brightness * lightColor * lightBias.y);
}

void main(void) {
	gl_Position = projection * view * transform * vec4(inPosition, 1.0);
	passColor = inColor.rgb * calculateLighting();
}