in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;
//������� ������
uniform vec3 viewPos;
//�������� �������
struct Material {
    //vec3 ambient;
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_diffuse4;
	sampler2D texture_diffuse5;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	sampler2D texture_specular4;
	sampler2D texture_specular5;
    float shininess;
};   
uniform Material material;
//������������ �������� �����
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
//�������� �������� �����
struct PointLight {    
    vec3 position;    
    float constant;
    float linear;
    float quadratic;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
//�������
struct SpotLight{
	vec3 direction;
	vec3 position;    
    float constant;
    float linear;
    float quadratic;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float cutOff;
	float outerCutOff;
};
uniform SpotLight spotLight;

//��������� ������� ������� ���������� �����
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 DiffuseTextureMixing();
vec3 SpecularTextureMixing();

void main(){
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = CalcDirLight(dirLight, Normal, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir);
	result += CalcSpotLight(spotLight, Normal, FragPos, viewDir);
	FragColor = vec4(result , 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient  = light.ambient  * DiffuseTextureMixing();
    vec3 diffuse  = light.diffuse  * diff * DiffuseTextureMixing();
    vec3 specular = light.specular * spec * SpecularTextureMixing();
    return (ambient + diffuse + specular);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient  = light.ambient  * DiffuseTextureMixing();
    vec3 diffuse  = light.diffuse  * diff * DiffuseTextureMixing();
    vec3 specular = light.specular * spec * SpecularTextureMixing();
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);	
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
	if(theta > light.outerCutOff) {
		float diff = max(dot(normal, lightDir), 0.0);
		diffuse = intensity*diff*light.diffuse  *DiffuseTextureMixing();		
		vec3 reflectDir = reflect(-lightDir, normal);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = intensity*light.specular * spec * SpecularTextureMixing();
	}
	vec3 ambient = intensity * light.ambient  * DiffuseTextureMixing();
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 DiffuseTextureMixing(){
	vec3 mixing = vec3(texture(material.texture_diffuse1, TexCoords));
	mixing+=vec3(texture(material.texture_diffuse2, TexCoords));
	mixing+=vec3(texture(material.texture_diffuse3, TexCoords));
	mixing+=vec3(texture(material.texture_diffuse4, TexCoords));
	mixing+=vec3(texture(material.texture_diffuse5, TexCoords));
	return mixing;
}

vec3 SpecularTextureMixing(){
	vec3 mixing = vec3(texture(material.texture_specular1, TexCoords));
	mixing+=vec3(texture(material.texture_specular2, TexCoords));
	mixing+=vec3(texture(material.texture_specular3, TexCoords));
	mixing+=vec3(texture(material.texture_specular4, TexCoords));
	mixing+=vec3(texture(material.texture_specular5, TexCoords));
	return mixing;
}