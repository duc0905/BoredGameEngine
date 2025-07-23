#version 330 core

in vec3 vNormal;
in vec3 vPos;
in vec2 vTexCoord;

uniform vec3 uLightDir = vec3(0.3, -1.0, 0.6); // Directional light direction (world space)
uniform vec3 uLightColor = vec3(0.7);
// uniform sampler2D uTexture;

out vec4 FragColor;

void main() {
    vec3 normal = normalize(vNormal);
    float diff = max(dot(normal, normalize(-uLightDir)), 0.0);
    vec3 diffuse = diff * uLightColor;

    // TODO: Specular lighting

    // TODO: Texture mapping
    // vec3 texColor = texture(uTexture, FragTexCoord).rgb;

    vec3 finalColor = diffuse + 0.1; // +0.1 ambient term

    FragColor = vec4(finalColor, 1.0);
}
