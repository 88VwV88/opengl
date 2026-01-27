#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform vec3 viewPos;
uniform Light light;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main() {
  vec3 lightDir = normalize(light.position - FragPos);
  // ambient shading
  vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
  // diffuse shading
  vec3  normal  = normalize(Normal);
  float diff    = max(dot(lightDir, normal), 0.0f);
  vec3  diffuse = light.diffuse * texture(texture_diffuse1, TexCoords).rgb;
  // specular shading
  vec3  reflectDir = reflect(-lightDir, normal);
  vec3  viewDir    = normalize(viewPos - FragPos);
  float spec       = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
  vec3 specular   = light.specular * spec * texture(texture_specular1, TexCoords).rgb;
  // attenuation
  float d           = length(light.position - FragPos);
  float attenuation = 1.0f / (light.constant + light.linear * d + light.quadratic * d * d);

  FragColor = vec4(ambient + (diffuse + specular) * attenuation, 1.0f);
}