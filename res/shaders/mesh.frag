#version 420 core

in vec3 vNormal;
in vec3 vPos;
in vec2 vTexCoord;
in vec3 vFragPos;

// uniform sampler2D uTexture;
uniform sampler2D uDiffuseMap;
uniform bool uHaveDiffuseMap;

uniform vec3 uEye;

out vec4 FragColor;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float strength;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};

#define MAX_LIGHT 1

uniform DirectionalLight dirLights[MAX_LIGHT];

uniform PointLight pointLights[MAX_LIGHT];

uniform Material uMaterial;

vec3 GetDiffuseColor(Material material) {
    return uHaveDiffuseMap ? texture(uDiffuseMap, vTexCoord).rgb : material.diffuse;
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, Material material) {
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);

    vec3 diffuseColor = GetDiffuseColor(material);

    return light.color * (diffuseColor * diff + uMaterial.specular * spec) + uMaterial.ambient;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, Material material) {
    vec3 lightDir = normalize(light.position - vFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float distance = length(light.position - vFragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);

    vec3 diffuseColor = GetDiffuseColor(material);

    return light.color * light.strength * (diffuseColor * diff + uMaterial.specular * spec) + uMaterial.ambient;
}

void main() {
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(uEye - vFragPos);

    vec3 finalColor = vec3(0.0);

    // Calculate directional light
    for (int i = 0; i < MAX_LIGHT; i++) {
        finalColor += CalcDirLight(dirLights[i], normal, viewDir, uMaterial);
    }

    // Calculate point lights
    for (int i = 0; i < MAX_LIGHT; i++) {
        finalColor += CalcPointLight(pointLights[i], normal, viewDir, uMaterial);
    }

    // TODO: Texture mapping
    // vec3 texColor = texture(uTexture, FragTexCoord).rgb;

    FragColor = vec4(finalColor, 1.0);
}
