in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 color;
uniform vec3 viewPos;
struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};   
uniform Material material;

struct Light {
	vec3 direction;	
    vec3 position;
	float cutOff;
	float outerCutOff;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
    float linear;
    float quadratic;
};
uniform Light light;  
uniform Light movinglight; 
uniform Light flashlight; 
void main()
{		
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 movambient  = movinglight.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 flashambient = flashlight.ambient  * vec3(texture(material.diffuse, TexCoords));	
		
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 flashdiffuse = vec3(0.0f);
	vec3 flashspecular = vec3(0.0f);	
	vec3 flashlightDir = normalize(flashlight.position - FragPos);
	float theta = dot(flashlightDir, normalize(-flashlight.direction));
	float epsilon   = flashlight.cutOff - flashlight.outerCutOff;
	float intensity = clamp((theta - flashlight.outerCutOff) / epsilon, 0.0, 1.0); 
	if(theta > flashlight.outerCutOff) {
		float flashdiff = max(dot(Normal, flashlightDir), 0.0);
		flashdiffuse = flashdiff*flashlight.diffuse  *vec3(texture(material.diffuse, TexCoords));
		
		vec3 flashreflectDir = reflect(-flashlightDir, Normal);  
		float flashspec = pow(max(dot(viewDir, flashreflectDir), 0.0), material.shininess);
		flashspecular = flashlight.specular * flashspec * vec3(texture(material.specular, TexCoords));
	}
	
	
	vec3 lightDir = normalize(-light.direction);
	vec3 movinglightDir = normalize(movinglight.position - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);	
    vec3 diffuse  = light.diffuse* diff * vec3(texture(material.diffuse, TexCoords));  
	
	float movdiff = max(dot(Normal, movinglightDir), 0.0);	
	vec3 movdiffuse = movdiff*movinglight.diffuse  *vec3(texture(material.diffuse, TexCoords)); 
	
    vec3 reflectDir = reflect(-lightDir, Normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	vec3 movreflectDir = reflect(-movinglightDir, Normal);  
    float movspec = pow(max(dot(viewDir, movreflectDir), 0.0), material.shininess);
    vec3 movspecular = movinglight.specular * movspec * vec3(texture(material.specular, TexCoords));
	
	float movdistance    = length(movinglight.position - FragPos);
	float movattenuation = 1.0 / (movinglight.constant + movinglight.linear * movdistance + movinglight.quadratic * (movdistance * movdistance));
	
	float flashdistance    = length(flashlight.position - FragPos);
	float flashattenuation = 1.0 / (flashlight.constant + flashlight.linear * flashdistance + flashlight.quadratic * (flashdistance * flashdistance));
   
   vec3 res1 = ambient + diffuse+ specular;   
   vec3 res2 = movambient + movdiffuse + movspecular;
   vec3 res3 = flashambient+ intensity*flashdiffuse + intensity*flashspecular;
color = vec4(res1+ movattenuation*res2 + flashattenuation*res3, 1.0f);
}