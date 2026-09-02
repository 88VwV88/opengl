#version 460 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    sampler2D diffuseMap;
    sampler2D specularMap;
    bool useDiffuseMap;
    bool useSpecularMap;

    float shininess;
};

struct Light {
    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoords;

out vec4 FragColor;

uniform Light uLight;
uniform Material uMaterial;
uniform vec3 uCameraPosition;

vec3 ComputeLighting() {
    vec3 normal = normalize(vNormal);
    vec3 viewDirection  = normalize(uCameraPosition - vFragPos);
    vec3 ligthDirection = normalize(uLight.position - vFragPos);
    vec3 reflectedDirection = reflect(-ligthDirection, normal);

    float diffuseStrength  = max(dot(normal, ligthDirection), 0.0f);
    float specularStrength = pow(max(dot(viewDirection, reflectedDirection), 0.0f), uMaterial.shininess);

    float d = length(uLight.position - vFragPos);
    float attenuation = 1.0f / (uLight.constant + d * uLight.linear + d * d * uLight.quadratic);
    vec3 diffuseColor = uMaterial.diffuse;
    vec3 specularColor = uMaterial.specular;

    if (uMaterial.useDiffuseMap) {
        diffuseColor *= texture(uMaterial.diffuseMap, vTexCoords).rgb;
    }

    if (uMaterial.useSpecularMap) {
        specularColor *= texture(uMaterial.specularMap, vTexCoords).rgb;
    }

    vec3 ambient  = uMaterial.ambient * diffuseColor;
    vec3 diffuse  = diffuseStrength * diffuseColor;
    vec3 specular = specularStrength * specularColor;

    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    FragColor = vec4(ComputeLighting(), 1.0f);
}
