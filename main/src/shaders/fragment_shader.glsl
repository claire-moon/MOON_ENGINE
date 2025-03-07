#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;

void main() {
	
	//diffuse lighting
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(Normal, lightDir), 0.0f);
	vec3 diffuse = diff * vec3(1.0, 0.5, 0.2); //orange
	
	FragColor = vec4(diffuse, 1.0);
}
