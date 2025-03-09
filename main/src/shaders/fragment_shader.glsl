#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform sampler2D texture1;

void main() {	

	//ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

	//diffuse lighting
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
	
	//tex color
	vec3 texColor = texture(texture1, FragPos.xy).rgb;
	
	//combine lighting and texture
	vec3 result = (ambient + diffuse) * texColor;
	FragColor = texture(texture1, TexCoord);
}
