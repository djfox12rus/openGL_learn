in vec3 Normal;
in vec3 FragPos;
out vec4 color;
uniform vec3 modelColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main()
{
float ambientStrength = 0.1f;
float specularStrength = 0.5f;

vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

vec3 ambient = ambientStrength * lightColor;

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
vec3 specular = specularStrength * spec * lightColor;

vec3 result = (ambient + diffuse + specular) * modelColor;
color = vec4(result, 1.0f);
}