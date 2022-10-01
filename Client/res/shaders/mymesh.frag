#version 330 core

in vec2 v_TexCoords;
in vec3 v_normal;
in vec3 v_FragPos;
uniform int u_ActorID;

layout (location = 0) out vec4 color;
layout (location = 1) out int actorID;

uniform sampler2D sampler;

// Ambient light
uniform vec3 u_ambientColor = vec3(1.0, 1.0, 1.0);
uniform float u_ambientStrength = 1.0;

// Diffuse light
uniform vec3 u_diffuseColor = vec3(0.0, 0.0, 0.0);
uniform vec3 u_lightPos;

// Specular light
uniform vec3 u_viewPos;
uniform vec3 u_specularColor = vec3(1.0, 1.0, 1.0);
uniform float u_specularStrength = 0.5;

vec3 getAmbient(vec3 color, float strength)
{
	return color * strength;
}

vec3 getDiffuse(vec3 color, vec3 pos, vec3 fragPos, vec3 normal)
{
	vec3 lightDir = normalize(pos - fragPos);  
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * color;
	return diffuse;
}

vec3 getSpecular(vec3 lightPos, vec3 lightColor, float specularStrength, vec3 fragPos, vec3 norm, vec3 viewPos)
{
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor;  
	return specular;
}

void main()
{
	vec3 ambient = getAmbient(u_ambientColor, u_ambientStrength);
	vec3 diffuse = getDiffuse(u_diffuseColor, u_lightPos, v_FragPos, v_normal);
	vec3 specular = getSpecular(u_lightPos, u_specularColor, u_specularStrength, v_FragPos, v_normal,u_viewPos);
	color = vec4(ambient + diffuse + specular, 1.0) * texture2D(sampler, v_TexCoords);
	actorID = u_ActorID;
}