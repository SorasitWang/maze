#version 330 core
out vec4 FragColor;

struct Material { 
    vec3 _diffuse;
    sampler2D diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;


    vec3 direction;
    float cutOff;
    float outerCutOff;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform Light spotLight;
uniform sampler2D shadowMap;

vec3 calculate(Light light,vec3 Normal, vec3 viewPos,vec3 FragPos,vec2 TexCoords);
float ShadowCalculation(vec4 fragPosLightSpace,float bias);

void main()
{
    vec3 result = calculate(light,Normal,viewPos,FragPos,TexCoords) +calculate(spotLight,Normal,viewPos,FragPos, TexCoords); 
    
    FragColor = vec4(result,1.0);
} 

vec3 calculate(Light light,vec3 Normal, vec3 viewPos,vec3 FragPos,vec2 TexCoords){

   vec3 lightDir = normalize(light.position - FragPos);
   float theta = dot(lightDir, normalize(-light.direction));
   float epsilon = light.cutOff - light.outerCutOff;
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // ambient
   vec3 ambient =  vec3(texture(material.diffuse,TexCoords)) * light.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff *vec3(texture(material.diffuse,TexCoords));;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
    float shadow = ShadowCalculation(FragPosLightSpace,bias);

    diffuse *= intensity;
    specular *= intensity;
    vec3 result = ambient+ ((1.0-shadow) * (diffuse + specular));

   // vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
    return result;

}

float ShadowCalculation(vec4 fragPosLightSpace,float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    
    float shadow = 0.0;
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) *
            texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}