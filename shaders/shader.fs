#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;

  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0f);
  // specular shading
  vec3  reflectDir = reflect(-lightDir, normal);
  float spec       = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess * 128.0f);
  // phong lighting
  vec3 ambient  = light.ambient         * texture(material.diffuse,  TexCoords).rgb;
  vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse,  TexCoords).rgb;
  vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

  return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0f);
  // specular shading
  vec3  reflectDir = reflect(-lightDir, normal);
  float spec       = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess * 128.0f);
  // attenuation
  float d           = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * d + light.quadratic * (d * d));    
  // phong lighting
  vec3 ambient  = light.ambient         * texture(material.diffuse,  TexCoords).rgb;
  vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse,  TexCoords).rgb;
  vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

  return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);

  // ambient shading
  vec3  ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  // diffuse shading
  float diff    = max(dot(normal, lightDir), 0.0f);
  vec3  diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
  // specular shading
  vec3  reflectDir = reflect(-lightDir, normal);
  float spec       = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess * 128.0f);
  vec3  specular   = light.specular * spec * texture(material.specular, TexCoords).rgb;
  // attenuation
  float d           = length(light.position - FragPos);
  float attenuation = 1.0f / (light.constant + light.linear * d + light.quadratic * d * d);
  // intensity
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

  return  (ambient + diffuse + specular) * attenuation * intensity;
}

void main() {
  vec3 normal = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  
  // diffuse light
  vec3 lighting = CalcDirLight(dirLight, normal, viewDir);
  // point lights
  for (int i = 0; i < NR_POINT_LIGHTS; ++i)
    lighting += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
  // spot light
  lighting += CalcSpotLight(spotLight, normal, FragPos, viewDir);

  FragColor = vec4(lighting, 1.0f);
}